
#pragma once

#include "database.h"

#include <memory>
#include <string>

enum class MeasurementStatus {
    Received,
    Verified,
    Processed,
    Invalid
};

std::string statusToString(MeasurementStatus status);

MeasurementStatus statusFromString(const std::string& status);

class MeasurementState {
public:
    virtual ~MeasurementState() = default;

    virtual bool canTransitionTo(MeasurementStatus next) const = 0;
};

class ReceivedState : public MeasurementState {
public:
    bool canTransitionTo(MeasurementStatus next) const override;
};

class VerifiedState : public MeasurementState {
public:
    bool canTransitionTo(MeasurementStatus next) const override;
};

class ProcessedState : public MeasurementState {
public:
    bool canTransitionTo(MeasurementStatus next) const override;
};

class InvalidState : public MeasurementState {
public:
    bool canTransitionTo(MeasurementStatus next) const override;
};

std::unique_ptr<MeasurementState> createMeasurementState(
    MeasurementStatus status
);

class MeasurementStateManager {
private:
    Storage& storage;

public:
    explicit MeasurementStateManager(Storage& storage);

    void transition(int measurementId, MeasurementStatus newStatus);
};
