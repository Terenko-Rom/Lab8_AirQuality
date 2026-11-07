#pragma once

#include <string>

struct Sensor {
    int id;
    std::string name;
    std::string location;
    std::string model;
    std::string status;
};

struct AirQualityIndicator {
    int id;
    int sensorId;
    std::string measuredAt;
    double pm25;
    double pm10;
    double co2;
    int aqi;
};