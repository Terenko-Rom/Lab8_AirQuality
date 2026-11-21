
#pragma once

#include <memory>
#include <string>
#include <vector>

enum class AirQualityEventType {
    NewMeasurement,
    MeasurementStateChanged
};

struct AirQualityEvent {
    AirQualityEventType type;
    int measurementId;
    std::string description;
};

class IAirQualityObserver {
public:
    virtual ~IAirQualityObserver() = default;

    virtual void onEvent(const AirQualityEvent& event) = 0;
};

class ConsoleLogger : public IAirQualityObserver {
public:
    void onEvent(const AirQualityEvent& event) override;
};

class AlertSystem : public IAirQualityObserver {
public:
    void onEvent(const AirQualityEvent& event) override;
};

class AirQualityEventManager {
private:
    std::vector<std::shared_ptr<IAirQualityObserver>> observers;

public:
    void attach(const std::shared_ptr<IAirQualityObserver>& observer);
    void detach(const std::shared_ptr<IAirQualityObserver>& observer);

    void notify(const AirQualityEvent& event);

    size_t getObserverCount() const;
};
