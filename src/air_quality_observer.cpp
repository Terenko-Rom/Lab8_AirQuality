
#include "air_quality_observer.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

void ConsoleLogger::onEvent(const AirQualityEvent& event) {
    std::cout << "[ConsoleLogger] ";

    if (event.type == AirQualityEventType::NewMeasurement) {
        std::cout << "New measurement received. ";
    }
    else {
        std::cout << "Measurement state changed. ";
    }

    std::cout << "ID: " << event.measurementId
        << ", Details: " << event.description << '\n';
}

void AlertSystem::onEvent(const AirQualityEvent& event) {
    std::cout << "[AlertSystem] ";

    if (event.type == AirQualityEventType::NewMeasurement) {
        std::cout << "New measurement notification. ";
    }
    else {
        std::cout << "State change notification. ";
    }

    std::cout << "Measurement ID: " << event.measurementId
        << ", Event: " << event.description << '\n';
}

void AirQualityEventManager::attach(
    const std::shared_ptr<IAirQualityObserver>& observer) {

    if (!observer) {
        throw std::invalid_argument("Observer cannot be null");
    }

    auto it = std::find(observers.begin(), observers.end(), observer);

    if (it == observers.end()) {
        observers.push_back(observer);
    }
}

void AirQualityEventManager::detach(
    const std::shared_ptr<IAirQualityObserver>& observer) {

    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
}

void AirQualityEventManager::notify(const AirQualityEvent& event) {
    for (const auto& observer : observers) {
        observer->onEvent(event);
    }
}

size_t AirQualityEventManager::getObserverCount() const {
    return observers.size();
}
