
#pragma once

#include "models.h"

#include <sqlite_orm/sqlite_orm.h>
#include <string>

inline auto createStorage(const std::string& path) {
    using namespace sqlite_orm;

    return make_storage(
        path,

        make_table(
            "sensors",
            make_column("id", &Sensor::id, primary_key().autoincrement()),
            make_column("name", &Sensor::name),
            make_column("location", &Sensor::location),
            make_column("model", &Sensor::model),
            make_column("status", &Sensor::status)
        ),

        make_table(
            "air_quality_indicators",
            make_column("id", &AirQualityIndicator::id, primary_key().autoincrement()),
            make_column("sensor_id", &AirQualityIndicator::sensorId),
            make_column("measured_at", &AirQualityIndicator::measuredAt),
            make_column("pm25", &AirQualityIndicator::pm25),
            make_column("pm10", &AirQualityIndicator::pm10),
            make_column("co2", &AirQualityIndicator::co2),
            make_column("aqi", &AirQualityIndicator::aqi),
            foreign_key(&AirQualityIndicator::sensorId).references(&Sensor::id)
        )
    );
}

using Storage = decltype(createStorage(""));

Storage& getStorage();
void initializeDatabase();
