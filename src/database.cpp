
#include "database.h"

Storage& getStorage() {
    static Storage storage = createStorage("air_quality.db");
    return storage;
}

void initializeDatabase() {
    auto& storage = getStorage();

    storage.sync_schema();

    if (storage.count<Sensor>() == 0) {
        storage.insert(Sensor{ 0, "Sensor A", "Kyiv, Center", "AirMonitor X1", "Active" });
        storage.insert(Sensor{ 0, "Sensor B", "Kyiv, Podil", "AirMonitor X2", "Active" });
        storage.insert(Sensor{ 0, "Sensor C", "Lviv, Center", "AirMonitor X1", "Active" });
        storage.insert(Sensor{ 0, "Sensor D", "Odesa, Center", "AirMonitor X3", "Inactive" });
        storage.insert(Sensor{ 0, "Sensor E", "Kharkiv, Center", "AirMonitor X2", "Active" });

        storage.insert(AirQualityIndicator{ 0, 1, "2026-11-01 08:00:00", 12.5, 25.0, 420.0, 45 });
        storage.insert(AirQualityIndicator{ 0, 1, "2026-11-01 12:00:00", 15.2, 30.0, 450.0, 52 });
        storage.insert(AirQualityIndicator{ 0, 1, "2026-11-01 18:00:00", 18.0, 35.0, 480.0, 60 });

        storage.insert(AirQualityIndicator{ 0, 2, "2026-11-01 08:00:00", 10.0, 20.0, 400.0, 38 });
        storage.insert(AirQualityIndicator{ 0, 2, "2026-11-01 12:00:00", 14.0, 28.0, 430.0, 48 });
        storage.insert(AirQualityIndicator{ 0, 2, "2026-11-01 18:00:00", 16.5, 32.0, 460.0, 55 });

        storage.insert(AirQualityIndicator{ 0, 3, "2026-11-01 08:00:00", 8.0, 15.0, 390.0, 30 });
        storage.insert(AirQualityIndicator{ 0, 3, "2026-11-01 12:00:00", 11.5, 22.0, 410.0, 40 });
        storage.insert(AirQualityIndicator{ 0, 3, "2026-11-01 18:00:00", 13.0, 26.0, 440.0, 46 });

        storage.insert(AirQualityIndicator{ 0, 4, "2026-11-01 08:00:00", 25.0, 50.0, 600.0, 85 });
        storage.insert(AirQualityIndicator{ 0, 4, "2026-11-01 12:00:00", 30.0, 60.0, 650.0, 100 });
        storage.insert(AirQualityIndicator{ 0, 4, "2026-11-01 18:00:00", 28.0, 55.0, 620.0, 92 });

        storage.insert(AirQualityIndicator{ 0, 5, "2026-11-01 08:00:00", 16.0, 32.0, 470.0, 55 });
        storage.insert(AirQualityIndicator{ 0, 5, "2026-11-01 12:00:00", 19.0, 38.0, 500.0, 65 });
        storage.insert(AirQualityIndicator{ 0, 5, "2026-11-01 18:00:00", 21.0, 42.0, 530.0, 72 });
    }
}
