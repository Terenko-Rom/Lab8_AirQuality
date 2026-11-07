
#pragma once

#include "database.h"

#include <vector>
#include <tuple>
#include <string>

class MeasurementRepository {
private:
    Storage& storage;

public:
    explicit MeasurementRepository(Storage& storage);

    int add(const AirQualityIndicator& measurement);
    std::vector<AirQualityIndicator> getAll();
    std::vector<AirQualityIndicator> getBySensorId(int sensorId);
    std::vector<AirQualityIndicator> getByMinimumAqi(int minAqi);
    std::vector<AirQualityIndicator> getSortedByPm25();
    std::vector<AirQualityIndicator> getLatest(int count);

    std::vector<std::tuple<int, std::string, int>> getMeasurementsWithSensorNames();

    double getAveragePm25();
    double getMaximumPm10();
    int getCount();

    bool testSuccessfulTransaction();
    bool testFailedTransaction();

    void remove(int id);
};
