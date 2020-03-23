/**
 * @file Snmpv3UserStore.cpp
 * @brief SNMPv3 User Store
 */

// Includes C/C++
#include <fstream>
#include <sstream>

// Own includes
#include "snmp/Snmpv3UserStore.h"

namespace NetMan {

/**
 * @brief Constructor for a SNMPv3 User Store
 */
Snmpv3UserStore::Snmpv3UserStore() {
    this->userTable = std::unordered_map<std::string, Snmpv3UserStoreEntry>();
}

/**
 * @brief Load a SNMPv3 user store from a file
 * @param path  File path
 */
void Snmpv3UserStore::load(const std::string &path) {

    // Open file
    this->path = path;
    std::ifstream infile;

    try {
        infile = std::ifstream(path);
    } catch(const std::ifstream::failure &e) {
        throw;
    }

    std::string line;
    Snmpv3UserStoreEntry entry;
    std::string name;

    // Read every line
    while (std::getline(infile, line)) {

        std::istringstream iss(line);
        if (iss >> name >> entry.authProto >> entry.authPass >> entry.privProto >> entry.privPass) {
            this->userTable[name] = entry;
        }
    }

    // Close file
    infile.close();
}

/**
 * @brief Add an user to the user store
 * @param name  User name
 * @param entry User parameters
 */
void Snmpv3UserStore::addUser(const std::string &name, const Snmpv3UserStoreEntry &entry) {
    try {
        if(this->userTable.find(name) != this->userTable.end()) {
            throw std::runtime_error("User " + name + " is repeated");
        }
        this->userTable[name] = entry;
    } catch (const std::runtime_error &e) {
        throw;
    } catch (const std::bad_alloc &e) {
        throw;
    }
}

/**
 * @brief Remove an user from the user store
 * @param name  User name
 */
void Snmpv3UserStore::removeUser(const std::string &name) {
    this->userTable.erase(name);
}

/**
 * @brief Get an user from the user store
 * @param name  User name
 * @return The user entry
 */
Snmpv3UserStoreEntry &Snmpv3UserStore::getUser(const std::string &name) {
    try {
        if(this->userTable.find(name) == this->userTable.end()) {
            throw std::runtime_error("User " + name + " not found");
        }
    } catch(const std::runtime_error &e) {
        throw;
    }

    return this->userTable[name];
}

/**
 * @brief Save the user store to a file
 */
void Snmpv3UserStore::save() {

    // Open the file
    std::ofstream outfile(this->path);

    // Write the user store
    char line[256];
    for(std::pair<std::string, Snmpv3UserStoreEntry> user : userTable) {
        Snmpv3UserStoreEntry &entry = user.second;
        sprintf(line, "%s %d %s %d %s", user.first.c_str(), entry.authProto, entry.authPass.c_str(), entry.privProto, entry.privPass.c_str());
        outfile.write(line, 256);
    }

    // Close the file
    outfile.close();
}

/**
 * @brief Destructor for a SNMPv3 user store
 */
Snmpv3UserStore::~Snmpv3UserStore() { }

/**
 * @brief Get the instance of the User Store
 * @return The singleton instance of the User Store
 */
Snmpv3UserStore &Snmpv3UserStore::getInstance() {
    static Snmpv3UserStore instance;
    return instance;
}

}