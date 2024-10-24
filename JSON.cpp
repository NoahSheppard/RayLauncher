#include "JSON.h"
#include <fstream>

using json = nlohmann::json;

std::string JSON::DefaultAccountFile = "accounts.json";

void JSON::AddAccountToFile(std::string account_id, std::string username, std::string deviceId, std::string secret, JSON::File file) {
    json jsonData;
    std::ifstream inFile(file == JSON::File::ACCOUNTS ? "accounts.json" : "");

    if (inFile.good()) {
        try { inFile >> jsonData; }
        catch (const json::parse_error& e) { jsonData = json::object(); }
        inFile.close();
    }

    json newEntry = {
        {"username", username},
        {"deviceId", deviceId},
        {"secret", secret}
    };

    json newArray = json::array();
    newArray.push_back(newEntry);

    jsonData[account_id] = newArray;

    std::ofstream outFile(file == JSON::File::ACCOUNTS ? "accounts.json" : "");
    outFile << std::setw(4) << jsonData << std::endl;
    outFile.close();
}

nlohmann::json JSON::GetAccountInformation(std::string account_id, JSON::File file) {
    json jsonData;

    std::ifstream inFile(file == JSON::File::ACCOUNTS ? "accounts.json" : "");
    if (!inFile.good()) {
        return nullptr;  // File doesn't exist
    }

    try {
        inFile >> jsonData;
    }
    catch (const json::parse_error& e) {
        return nullptr;  // Invalid JSON
    }

    // Check if var1 exists and has at least one entry
    if (!jsonData.contains(account_id) || jsonData[account_id].empty() || !jsonData[account_id].is_array()) {
        return nullptr;
    }

    // Get the first (and only) entry in the array
    const auto& entry = jsonData[account_id][0];

    // Check if all required fields exist
    if (!entry.contains("username") || !entry.contains("deviceId") || !entry.contains("secret")) {
        return nullptr;
    }

    // Return the json object directly
    return entry;
}