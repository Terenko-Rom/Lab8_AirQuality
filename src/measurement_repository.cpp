
#include "measurement_repository.h"

using namespace sqlite_orm;

MeasurementRepository::MeasurementRepository(Storage& storage)
    : storage(storage) {
}

int MeasurementRepository::add(const AirQualityIndicator& measurement) {
    return static_cast<int>(storage.insert(measurement));
}

std::vector<AirQualityIndicator> MeasurementRepository::getAll() {
    return storage.get_all<AirQualityIndicator>();
}

std::vector<AirQualityIndicator> MeasurementRepository::getBySensorId(int sensorId) {
    return storage.get_all<AirQualityIndicator>(
        where(c(&AirQualityIndicator::sensorId) == sensorId)
    );
}

std::vector<AirQualityIndicator> MeasurementRepository::getByMinimumAqi(int minAqi) {
    return storage.get_all<AirQualityIndicator>(
        where(c(&AirQualityIndicator::aqi) >= minAqi)
    );
}

std::vector<AirQualityIndicator> MeasurementRepository::getSortedByPm25() {
    return storage.get_all<AirQualityIndicator>(
        order_by(&AirQualityIndicator::pm25).desc()
    );
}

std::vector<AirQualityIndicator> MeasurementRepository::getLatest(int count) {
    return storage.get_all<AirQualityIndicator>(
        order_by(&AirQualityIndicator::measuredAt).desc(),
        limit(static_cast<unsigned int>(count))
    );
}

std::vector<std::tuple<int, std::string, int>>
MeasurementRepository::getMeasurementsWithSensorNames() {
    return storage.select(
        columns(
            &AirQualityIndicator::id,
            &Sensor::name,
            &AirQualityIndicator::aqi
        ),
        inner_join<Sensor>(
            on(c(&Sensor::id) == &AirQualityIndicator::sensorId)
        )
    );
}

std::vector<std::tuple<int, int>> MeasurementRepository::getCountBySensor() {
    return storage.select(
        columns(
            &AirQualityIndicator::sensorId,
            count(&AirQualityIndicator::id)
        ),
        group_by(&AirQualityIndicator::sensorId)
    );
}

double MeasurementRepository::getAveragePm25() {
    return storage.avg(&AirQualityIndicator::pm25);
}

double MeasurementRepository::getMaximumPm10() {
    auto result = storage.max(&AirQualityIndicator::pm10);
    return result ? *result : 0.0;
}

int MeasurementRepository::getCount() {
    return static_cast<int>(storage.count<AirQualityIndicator>());
}

bool MeasurementRepository::testSuccessfulTransaction() {
    return storage.transaction([&]() {
        int insertedId = static_cast<int>(
            storage.insert(AirQualityIndicator{
                0, 1, storage.current_timestamp(), 7.5, 14.0, 410.0, 25
                })
            );

        storage.remove<AirQualityIndicator>(insertedId);

        return true;
        });
}

bool MeasurementRepository::testFailedTransaction() {
    int countBefore = getCount();

    bool transactionResult = storage.transaction([&]() {
        try {
            storage.insert(AirQualityIndicator{
                0, 999999, storage.current_timestamp(), 50.0, 90.0, 900.0, 150
                });
        }
        catch (...) {
            return false;
        }

        return true;
        });

    return !transactionResult && getCount() == countBefore;
}

bool MeasurementRepository::testOperationsWithoutTransaction() {
    int countBefore = getCount();

    int insertedId = static_cast<int>(
        storage.insert(AirQualityIndicator{
            0, 1, storage.current_timestamp(), 9.5, 18.0, 420.0, 35
            })
        );

    storage.remove<AirQualityIndicator>(insertedId);

    return getCount() == countBefore;
}

void MeasurementRepository::remove(int id) {
    storage.remove<AirQualityIndicator>(id);
}
