
#include "database.h"
#include "sensor_repository.h"
#include "measurement_repository.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void compareSQLiteAndORM();

void fillDatabase(SensorRepository& sensors, MeasurementRepository& measurements) {
    if (!sensors.getAll().empty()) {
        return;
    }

    int sensor1 = sensors.add(Sensor{ 0, "Sensor A", "Kyiv, Center", "AirMonitor X1", "Active" });
    int sensor2 = sensors.add(Sensor{ 0, "Sensor B", "Kyiv, Podil", "AirMonitor X2", "Active" });
    int sensor3 = sensors.add(Sensor{ 0, "Sensor C", "Lviv, Center", "AirMonitor X1", "Active" });
    int sensor4 = sensors.add(Sensor{ 0, "Sensor D", "Odesa, Center", "AirMonitor X3", "Inactive" });
    int sensor5 = sensors.add(Sensor{ 0, "Sensor E", "Kharkiv, Center", "AirMonitor X2", "Active" });

    measurements.add(AirQualityIndicator{ 0, sensor1, "2026-11-01 08:00:00", 12.5, 25.0, 420.0, 45 });
    measurements.add(AirQualityIndicator{ 0, sensor1, "2026-11-01 12:00:00", 15.2, 30.0, 450.0, 52 });
    measurements.add(AirQualityIndicator{ 0, sensor1, "2026-11-01 18:00:00", 18.0, 35.0, 480.0, 60 });

    measurements.add(AirQualityIndicator{ 0, sensor2, "2026-11-01 08:00:00", 10.0, 20.0, 400.0, 38 });
    measurements.add(AirQualityIndicator{ 0, sensor2, "2026-11-01 12:00:00", 14.0, 28.0, 430.0, 48 });
    measurements.add(AirQualityIndicator{ 0, sensor2, "2026-11-01 18:00:00", 16.5, 32.0, 460.0, 55 });

    measurements.add(AirQualityIndicator{ 0, sensor3, "2026-11-01 08:00:00", 8.0, 15.0, 390.0, 30 });
    measurements.add(AirQualityIndicator{ 0, sensor3, "2026-11-01 12:00:00", 11.5, 22.0, 410.0, 40 });
    measurements.add(AirQualityIndicator{ 0, sensor3, "2026-11-01 18:00:00", 13.0, 26.0, 440.0, 46 });

    measurements.add(AirQualityIndicator{ 0, sensor4, "2026-11-01 08:00:00", 25.0, 50.0, 600.0, 85 });
    measurements.add(AirQualityIndicator{ 0, sensor4, "2026-11-01 12:00:00", 30.0, 60.0, 650.0, 100 });
    measurements.add(AirQualityIndicator{ 0, sensor4, "2026-11-01 18:00:00", 28.0, 55.0, 620.0, 92 });

    measurements.add(AirQualityIndicator{ 0, sensor5, "2026-11-01 08:00:00", 16.0, 32.0, 470.0, 55 });
    measurements.add(AirQualityIndicator{ 0, sensor5, "2026-11-01 12:00:00", 19.0, 38.0, 500.0, 65 });
    measurements.add(AirQualityIndicator{ 0, sensor5, "2026-11-01 18:00:00", 21.0, 42.0, 530.0, 72 });
}

int main() {
    try {
        initializeDatabase();

        auto& storage = getStorage();

        SensorRepository sensors(storage);
        MeasurementRepository measurements(storage);

        fillDatabase(sensors, measurements);

        cout << "AIR QUALITY MONITORING SYSTEM" << endl;
        cout << "=============================" << endl;

        cout << "\n1. ALL SENSORS" << endl;

        for (const auto& sensor : sensors.getAll()) {
            cout << "ID: " << sensor.id
                << ", Name: " << sensor.name
                << ", Location: " << sensor.location
                << ", Model: " << sensor.model
                << ", Status: " << sensor.status << endl;
        }

        cout << "\n2. ALL MEASUREMENTS" << endl;

        for (const auto& item : measurements.getAll()) {
            cout << "ID: " << item.id
                << ", Sensor ID: " << item.sensorId
                << ", Date: " << item.measuredAt
                << ", PM2.5: " << item.pm25
                << ", PM10: " << item.pm10
                << ", CO2: " << item.co2
                << ", AQI: " << item.aqi << endl;
        }

        cout << "\n3. MEASUREMENTS FOR SENSOR 1" << endl;

        for (const auto& item : measurements.getBySensorId(1)) {
            cout << "Date: " << item.measuredAt
                << ", PM2.5: " << item.pm25
                << ", AQI: " << item.aqi << endl;
        }

        cout << "\n4. MEASUREMENTS WITH AQI >= 70" << endl;

        for (const auto& item : measurements.getByMinimumAqi(70)) {
            cout << "Sensor ID: " << item.sensorId
                << ", AQI: " << item.aqi
                << ", PM2.5: " << item.pm25 << endl;
        }

        cout << "\n5. MEASUREMENTS SORTED BY PM2.5" << endl;

        for (const auto& item : measurements.getSortedByPm25()) {
            cout << "ID: " << item.id
                << ", PM2.5: " << item.pm25 << endl;
        }

        cout << "\n6. FIVE LATEST MEASUREMENTS" << endl;

        for (const auto& item : measurements.getLatest(5)) {
            cout << "Date: " << item.measuredAt
                << ", Sensor ID: " << item.sensorId
                << ", AQI: " << item.aqi << endl;
        }

        cout << "\n7. MEASUREMENTS WITH SENSOR NAMES (JOIN)" << endl;

        for (const auto& item : measurements.getMeasurementsWithSensorNames()) {
            cout << "Measurement ID: " << get<0>(item)
                << ", Sensor: " << get<1>(item)
                << ", AQI: " << get<2>(item) << endl;
        }

        cout << "\n8. AGGREGATE RESULTS" << endl;
        cout << "Average PM2.5: " << measurements.getAveragePm25() << endl;
        cout << "Maximum PM10: " << measurements.getMaximumPm10() << endl;
        cout << "Total measurements: " << measurements.getCount() << endl;

        cout << "\n9. MEASUREMENTS COUNT BY SENSOR (GROUP BY)" << endl;

        for (const auto& item : measurements.getCountBySensor()) {
            cout << "Sensor ID: " << get<0>(item)
                << ", Measurements: " << get<1>(item) << endl;
        }

        cout << "\n10. SENSOR CRUD OPERATIONS" << endl;

        int temporaryId = sensors.add(
            Sensor{ 0, "Temporary Sensor", "Test Location", "Test Model", "Active" }
        );

        auto temporarySensor = sensors.getById(temporaryId);

        if (temporarySensor.has_value()) {
            cout << "Sensor added: " << temporarySensor->name << endl;

            temporarySensor->status = "Inactive";
            sensors.update(*temporarySensor);

            cout << "Sensor updated: " << sensors.getById(temporaryId)->status << endl;

            sensors.remove(temporaryId);
            cout << "Sensor deleted." << endl;
        }

        cout << "\n11. TRANSACTION TESTS" << endl;

        bool successfulTransaction = measurements.testSuccessfulTransaction();
        cout << "Successful transaction: "
            << (successfulTransaction ? "COMMITTED" : "FAILED") << endl;

        bool failedTransaction = measurements.testFailedTransaction();
        cout << "Foreign key error and rollback: "
            << (failedTransaction ? "ROLLBACK SUCCESSFUL" : "ROLLBACK FAILED") << endl;

        cout << "\n12. OPERATIONS WITHOUT EXPLICIT TRANSACTION" << endl;

        bool noTransaction = measurements.testOperationsWithoutTransaction();

        cout << "Insert and delete without explicit transaction: "
            << (noTransaction ? "SUCCESSFUL" : "FAILED") << endl;

        cout << "\n13. FINAL DATABASE COUNTS" << endl;
        cout << "Sensors: " << sensors.getAll().size() << endl;
        cout << "Measurements: " << measurements.getCount() << endl;

        compareSQLiteAndORM();

        cout << "\nProgram completed successfully." << endl;
    }
    catch (const exception& error) {
        cerr << "Error: " << error.what() << endl;
        return 1;
    }

    return 0;
}
