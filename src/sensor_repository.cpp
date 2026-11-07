
#include "sensor_repository.h"

SensorRepository::SensorRepository(Storage& storage)
    : storage(storage) {
}

int SensorRepository::add(const Sensor& sensor) {
    return static_cast<int>(storage.insert(sensor));
}

std::vector<Sensor> SensorRepository::getAll() {
    return storage.get_all<Sensor>();
}

std::optional<Sensor> SensorRepository::getById(int id) {
    return storage.get_optional<Sensor>(id);
}

void SensorRepository::update(const Sensor& sensor) {
    storage.update(sensor);
}

void SensorRepository::remove(int id) {
    storage.remove<Sensor>(id);
}
