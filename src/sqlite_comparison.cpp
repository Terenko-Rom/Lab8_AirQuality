#include "database.h"

#include <sqlite3.h>
#include <iostream>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

void compareSQLiteAndORM() {
    cout << "\n12. SQLITE C API VS ORM COMPARISON" << endl;

    sqlite3* database = nullptr;

    int openResult = sqlite3_open("air_quality.db", &database);

    if (openResult != SQLITE_OK) {
        cerr << "SQLite connection error: "
            << sqlite3_errmsg(database) << endl;

        if (database != nullptr) {
            sqlite3_close(database);
        }

        return;
    }

    sqlite3_stmt* statement = nullptr;

    const char* sql =
        "SELECT COUNT(*) FROM air_quality_indicators;";

    int prepareResult = sqlite3_prepare_v2(
        database,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (prepareResult != SQLITE_OK) {
        cerr << "SQL query error: "
            << sqlite3_errmsg(database) << endl;

        sqlite3_close(database);
        return;
    }

    int sqliteCount = 0;
    const int iterations = 1000;

    auto sqliteStart = high_resolution_clock::now();

    for (int i = 0; i < iterations; i++) {
        if (sqlite3_step(statement) == SQLITE_ROW) {
            sqliteCount = sqlite3_column_int(statement, 0);
        }

        sqlite3_reset(statement);
    }

    auto sqliteEnd = high_resolution_clock::now();

    sqlite3_finalize(statement);
    sqlite3_close(database);

    auto sqliteTime = duration_cast<microseconds>(
        sqliteEnd - sqliteStart
    ).count();

    auto& storage = getStorage();

    int ormCount = 0;

    auto ormStart = high_resolution_clock::now();

    for (int i = 0; i < iterations; i++) {
        ormCount = static_cast<int>(
            storage.count<AirQualityIndicator>()
            );
    }

    auto ormEnd = high_resolution_clock::now();

    auto ormTime = duration_cast<microseconds>(
        ormEnd - ormStart
    ).count();

    cout << "Number of iterations: " << iterations << endl;

    cout << "\nSQLite C API:" << endl;
    cout << "Records: " << sqliteCount << endl;
    cout << "Execution time: " << sqliteTime << " microseconds" << endl;

    cout << "\nSQLite ORM:" << endl;
    cout << "Records: " << ormCount << endl;
    cout << "Execution time: " << ormTime << " microseconds" << endl;

    cout << "\nComparison completed." << endl;
}