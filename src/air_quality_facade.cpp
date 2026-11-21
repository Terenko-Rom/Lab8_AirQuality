
#include "air_quality_facade.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

AirQualityFacade::AirQualityFacade(Storage& storage)
    : sensorRepository(storage),
    measurementRepository(storage),
    analyzer(std::make_unique<AbsoluteDeviationStrategy>()) {

    logger = std::make_shared<ConsoleLogger>();
    alertSystem = std::make_shared<AlertSystem>();

    eventManager.attach(logger);
    eventManager.attach(alertSystem);
}

std::vector<Sensor> AirQualityFacade::getAllSensors() {
    return sensorRepository.getAll();
}

std::vector<AirQualityIndicator> AirQualityFacade::getAllMeasurements() {
    return measurementRepository.getAll();
}

int AirQualityFacade::addMeasurement(
    const AirQualityIndicator& measurement
) {
    int measurementId = measurementRepository.add(measurement);

    AirQualityEvent event{
        AirQualityEventType::NewMeasurement,
        measurementId,
        "PM2.5: " + std::to_string(measurement.pm25) +
        ", PM10: " + std::to_string(measurement.pm10) +
        ", AQI: " + std::to_string(measurement.aqi)
    };

    eventManager.notify(event);

    return measurementId;
}

void AirQualityFacade::removeMeasurement(int measurementId) {
    measurementRepository.remove(measurementId);
}

double AirQualityFacade::analyzeMeasurement(
    int measurementId,
    double standardValue,
    double allowedDeviation
) {
    auto measurements = measurementRepository.getAll();

    auto measurement = std::find_if(
        measurements.begin(),
        measurements.end(),
        [measurementId](const AirQualityIndicator& item) {
            return item.id == measurementId;
        }
    );

    if (measurement == measurements.end()) {
        throw std::runtime_error("Measurement not found");
    }

    return analyzer.analyze(
        measurement->pm25,
        standardValue,
        allowedDeviation
    );
}

void AirQualityFacade::setAnalysisStrategy(
    std::unique_ptr<IAirQualityStrategy> strategy
) {
    analyzer.setStrategy(std::move(strategy));
}

void AirQualityFacade::notifyStateChanged(
    int measurementId,
    const std::string& previousState,
    const std::string& newState
) {
    AirQualityEvent event{
        AirQualityEventType::MeasurementStateChanged,
        measurementId,
        previousState + " -> " + newState
    };

    eventManager.notify(event);
}

void AirQualityFacade::attachObserver(
    const std::shared_ptr<IAirQualityObserver>& observer
) {
    eventManager.attach(observer);
}

void AirQualityFacade::detachObserver(
    const std::shared_ptr<IAirQualityObserver>& observer
) {
    eventManager.detach(observer);
}

size_t AirQualityFacade::getObserverCount() const {
    return eventManager.getObserverCount();
}
