
#include "air_quality_strategy.h"

#include <cmath>
#include <stdexcept>
#include <utility>

double AbsoluteDeviationStrategy::calculate(
    double currentValue,
    double standardValue,
    double allowedDeviation
) const {
    return std::abs(currentValue - standardValue);
}

std::string AbsoluteDeviationStrategy::getName() const {
    return "Absolute Deviation Strategy";
}

double RelativeDeviationStrategy::calculate(
    double currentValue,
    double standardValue,
    double allowedDeviation
) const {
    if (standardValue == 0.0) {
        throw std::invalid_argument(
            "Standard value cannot be zero for relative deviation"
        );
    }

    return std::abs(currentValue - standardValue)
        / std::abs(standardValue) * 100.0;
}

std::string RelativeDeviationStrategy::getName() const {
    return "Relative Deviation Strategy";
}

double NormalizedDeviationStrategy::calculate(
    double currentValue,
    double standardValue,
    double allowedDeviation
) const {
    if (allowedDeviation <= 0.0) {
        throw std::invalid_argument(
            "Allowed deviation must be greater than zero"
        );
    }

    return std::abs(currentValue - standardValue)
        / allowedDeviation;
}

std::string NormalizedDeviationStrategy::getName() const {
    return "Normalized Deviation Strategy";
}

AirQualityAnalyzer::AirQualityAnalyzer(
    std::unique_ptr<IAirQualityStrategy> strategy
) : strategy(std::move(strategy)) {
    if (!this->strategy) {
        throw std::invalid_argument("Strategy cannot be null");
    }
}

void AirQualityAnalyzer::setStrategy(
    std::unique_ptr<IAirQualityStrategy> newStrategy
) {
    if (!newStrategy) {
        throw std::invalid_argument("Strategy cannot be null");
    }

    strategy = std::move(newStrategy);
}

double AirQualityAnalyzer::analyze(
    double currentValue,
    double standardValue,
    double allowedDeviation
) const {
    return strategy->calculate(
        currentValue,
        standardValue,
        allowedDeviation
    );
}

std::string AirQualityAnalyzer::getStrategyName() const {
    return strategy->getName();
}
