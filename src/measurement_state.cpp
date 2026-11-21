
#include "measurement_state.h"

#include <stdexcept>
#include <utility>

std::string statusToString(MeasurementStatus status) {
    switch (status) {
    case MeasurementStatus::Received:
        return "Отриманий";

    case MeasurementStatus::Verified:
        return "Перевірений";

    case MeasurementStatus::Processed:
        return "Опрацьований";

    case MeasurementStatus::Invalid:
        return "Некоректний";
    }

    throw std::invalid_argument("Unknown measurement status");
}

MeasurementStatus statusFromString(const std::string& status) {
    if (status == "Отриманий") {
        return MeasurementStatus::Received;
    }

    if (status == "Перевірений") {
        return MeasurementStatus::Verified;
    }

    if (status == "Опрацьований") {
        return MeasurementStatus::Processed;
    }

    if (status == "Некоректний") {
        return MeasurementStatus::Invalid;
    }

    throw std::invalid_argument("Unknown stored measurement status");
}

bool ReceivedState::canTransitionTo(MeasurementStatus next) const {
    return next == MeasurementStatus::Verified ||
        next == MeasurementStatus::Invalid;
}

bool VerifiedState::canTransitionTo(MeasurementStatus next) const {
    return next == MeasurementStatus::Processed ||
        next == MeasurementStatus::Invalid;
}

bool ProcessedState::canTransitionTo(MeasurementStatus) const {
    return false;
}

bool InvalidState::canTransitionTo(MeasurementStatus) const {
    return false;
}

std::unique_ptr<MeasurementState> createMeasurementState(
    MeasurementStatus status
) {
    switch (status) {
    case MeasurementStatus::Received:
        return std::make_unique<ReceivedState>();

    case MeasurementStatus::Verified:
        return std::make_unique<VerifiedState>();

    case MeasurementStatus::Processed:
        return std::make_unique<ProcessedState>();

    case MeasurementStatus::Invalid:
        return std::make_unique<InvalidState>();
    }

    throw std::invalid_argument("Cannot create measurement state");
}

MeasurementStateManager::MeasurementStateManager(Storage& storage)
    : storage(storage) {
}

void MeasurementStateManager::transition(
    int measurementId,
    MeasurementStatus newStatus
) {
    storage.transaction([&]() {
        auto measurement = storage.get<AirQualityIndicator>(measurementId);

        MeasurementStatus currentStatus =
            statusFromString(measurement.state);

        auto currentState = createMeasurementState(currentStatus);

        if (!currentState->canTransitionTo(newStatus)) {
            throw std::invalid_argument(
                "Transition from " + measurement.state +
                " to " + statusToString(newStatus) +
                " is not allowed"
            );
        }

        std::string previousState = measurement.state;
        std::string nextState = statusToString(newStatus);

        measurement.state = nextState;

        storage.update(measurement);

        storage.insert(
            MeasurementStateHistory{
                0,
                measurementId,
                previousState,
                nextState,
                storage.current_timestamp()
            }
        );

        return true;
        });
}
