
#pragma once

#include "database.h"

#include <vector>
#include <optional>

class SensorRepository {
private:
    Storage& storage;

public:
    explicit SensorRepository(Storage& storage);

    int add(const Sensor& sensor);
    std::vector<Sensor> getAll();
    std::optional<Sensor> getById(int id);
    void update(const Sensor& sensor);
    void remove(int id);
};
