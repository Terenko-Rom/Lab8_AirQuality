
#include "state_demo.h"
#include "measurement_state.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

void runStateDemo(Storage& storage) {
    std::cout << "\n14. STATE PATTERN DEMONSTRATION\n";
    std::cout << "==============================\n";

    auto allMeasurements = storage.get_all<AirQualityIndicator>();

    auto measurement = std::find_if(
        allMeasurements.begin(),
        allMeasurements.end(),
        [](const AirQualityIndicator& item) {
            return item.state == "Отриманий";
        }
    );

    if (measurement == allMeasurements.end()) {
        std::cout << "No measurements available for processing.\n";
        return;
    }

    int measurementId = measurement->id;

    MeasurementStateManager manager(storage);

    std::cout << "Measurement ID: " << measurementId << '\n';
    std::cout << "Initial state: Received\n";

    manager.transition(
        measurementId,
        MeasurementStatus::Verified
    );

    std::cout << "New state: Verified\n";

    manager.transition(
        measurementId,
        MeasurementStatus::Processed
    );

    std::cout << "New state: Processed\n";

    std::cout << "\nAttempting invalid transition...\n";

    try {
        manager.transition(
            measurementId,
            MeasurementStatus::Verified
        );

        std::cout << "ERROR: Invalid transition was accepted.\n";
    }
    catch (const std::exception&) {
        std::cout << "Invalid transition rejected: "
            << "Transition from Processed to Verified is not allowed.\n";
    }

    std::cout << "\nSTATE HISTORY\n";

    auto history = storage.get_all<MeasurementStateHistory>();

    for (const auto& entry : history) {
        if (entry.measurementId == measurementId) {
            std::string previousState = entry.previousState;
            std::string newState = entry.newState;

            if (previousState == "Отриманий") previousState = "Received";
            else if (previousState == "Перевірений") previousState = "Verified";
            else if (previousState == "Опрацьований") previousState = "Processed";
            else if (previousState == "Некоректний") previousState = "Invalid";

            if (newState == "Отриманий") newState = "Received";
            else if (newState == "Перевірений") newState = "Verified";
            else if (newState == "Опрацьований") newState = "Processed";
            else if (newState == "Некоректний") newState = "Invalid";

            std::cout << previousState
                << " -> "
                << newState
                << " | "
                << entry.changedAt
                << '\n';
        }
    }
}
