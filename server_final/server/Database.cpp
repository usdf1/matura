#include "database/sqlite3.h" 
#include <iostream>
#include <sstream>
#include "Database.h"

static sqlite3* db = nullptr;

bool initializeDatabase() {
    int rc = sqlite3_open("game.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    else {
        std::cout << "Database opened successfully!" << std::endl;
    }

    // Create the users table with additional columns for skins.
    const char* createTableSQL =
        "CREATE TABLE IF NOT EXISTS users ("
        "username TEXT PRIMARY KEY, "
        "password TEXT, "
        "carSkin TEXT DEFAULT 'red', "
        "tankSkin TEXT DEFAULT 'blue'"
        ");";

    char* errMsg = nullptr;
    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error (creating table): " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool registerUser(const std::string& username, const std::string& password) {
    // Check if the username already exists.
    std::string sqlCheck = "SELECT COUNT(*) FROM users WHERE username = ?;";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, sqlCheck.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement (check user): " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::cerr << "Failed to execute statement (check user): " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    if (count > 0) {
        // Username already exists.
        return false;
    }

    // Insert the new user with default skins ("blue").
    std::string sqlInsert = "INSERT INTO users (username, password, carSkin, tankSkin) VALUES (?, ?, 'blue', 'blue');";
    rc = sqlite3_prepare_v2(db, sqlInsert.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement (insert user): " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool loginUser(const std::string& username, const std::string& password) {
    std::string sqlQuery = "SELECT password FROM users WHERE username = ?;";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement (login): " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        const unsigned char* dbPassword = sqlite3_column_text(stmt, 0);
        bool valid = dbPassword && password == reinterpret_cast<const char*>(dbPassword);
        sqlite3_finalize(stmt);
        return valid;
    }

    sqlite3_finalize(stmt);
    return false;
}

bool updateSkins(const std::string& username, const std::string& carSkin, const std::string& tankSkin) {
    std::string sqlUpdate = "UPDATE users SET carSkin = ?, tankSkin = ? WHERE username = ?;";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sqlUpdate.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement (update skins): " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, carSkin.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, tankSkin.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Update failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool getSkins(const std::string& username, std::string& carSkin, std::string& tankSkin) {
    std::string sqlQuery = "SELECT carSkin, tankSkin FROM users WHERE username = ?;";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement (getSkins): " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char* cs = sqlite3_column_text(stmt, 0);
        const unsigned char* ts = sqlite3_column_text(stmt, 1);
        if (cs)
            carSkin = reinterpret_cast<const char*>(cs);
        if (ts)
            tankSkin = reinterpret_cast<const char*>(ts);
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

void closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}
