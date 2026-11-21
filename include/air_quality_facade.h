
#pragma once

#include "database.h"
#include "sensor_repository.h"
#include "measurement_repository.h"
#include "air_quality_strategy.h"
#include "air_quality_observer.h"

#include <memory>
#include <string>
#include <vector>

class AirQualityFacade {
private:
    SensorRepository sensorRepository;
    MeasurementRepository measurementRepository;

    AirQualityAnalyzer analyzer;
    AirQualityEventManager eventManager;

    std::shared_ptr<ConsoleLogger> logger;
    std::shared_ptr<AlertSystem> alertSystem;

public:
    explicit AirQualityFacade(Storage& storage);

    std::vector<Sensor> getAllSensors();
    std::vector<AirQualityIndicator> getAllMeasurements();

    int addMeasurement(const AirQualityIndicator& measurement);
    void removeMeasurement(int measurementId);

    double analyzeMeasurement(
        int measurementId,
        double standardValue,
        double allowedDeviation
    );

    void setAnalysisStrategy(
        std::unique_ptr<IAirQualityStrategy> strategy
    );

    void notifyStateChanged(
        int measurementId,
        const std::string& previousState,
        const std::string& newState
    );

    void attachObserver(
        const std::shared_ptr<IAirQualityObserver>& observer
    );

    void detachObserver(
        const std::shared_ptr<IAirQualityObserver>& observer
    );

    size_t getObserverCount() const;
};
