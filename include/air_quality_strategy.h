
#pragma once

#include <memory>
#include <string>

class IAirQualityStrategy {
public:
    virtual ~IAirQualityStrategy() = default;

    virtual double calculate(
        double currentValue,
        double standardValue,
        double allowedDeviation
    ) const = 0;

    virtual std::string getName() const = 0;
};

class AbsoluteDeviationStrategy : public IAirQualityStrategy {
public:
    double calculate(
        double currentValue,
        double standardValue,
        double allowedDeviation
    ) const override;

    std::string getName() const override;
};

class RelativeDeviationStrategy : public IAirQualityStrategy {
public:
    double calculate(
        double currentValue,
        double standardValue,
        double allowedDeviation
    ) const override;

    std::string getName() const override;
};

class NormalizedDeviationStrategy : public IAirQualityStrategy {
public:
    double calculate(
        double currentValue,
        double standardValue,
        double allowedDeviation
    ) const override;

    std::string getName() const override;
};

class AirQualityAnalyzer {
private:
    std::unique_ptr<IAirQualityStrategy> strategy;

public:
    explicit AirQualityAnalyzer(
        std::unique_ptr<IAirQualityStrategy> strategy
    );

    void setStrategy(std::unique_ptr<IAirQualityStrategy> newStrategy);

    double analyze(
        double currentValue,
        double standardValue,
        double allowedDeviation
    ) const;

    std::string getStrategyName() const;
};
