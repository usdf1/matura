#pragma once

#ifndef DATABASE_H
#define DATABASE_H

#include <string>

// Initializes the database and creates the users table if it doesn't exist.
// Returns true on success, false otherwise.
bool initializeDatabase();

// Attempts to register a new user (username, password).
// Returns true if registration was successful (username did not exist),
// false if the username already exists or on an error.
bool registerUser(const std::string& username, const std::string& password);
bool loginUser(const std::string& username, const std::string& password);

bool updateSkins(const std::string& username, const std::string& carSkin, const std::string& tankSkin);
bool getSkins(const std::string& username, std::string& carSkin, std::string& tankSkin);
// Closes the database connection.
void closeDatabase();

#endif // DATABASE_H
