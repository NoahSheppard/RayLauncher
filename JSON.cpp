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

void JSON::WriteDebugToFile(std::string fullFilePath, nlohmann::json jsonData) {
    std::ifstream inFile(fullFilePath);

    if (inFile.good()) {
        try { inFile >> jsonData; }
        catch (const json::parse_error& e) { jsonData = json::object(); }
        inFile.close();
    }

    std::ofstream outFile(fullFilePath);
    outFile << std::setw(4) << jsonData << std::endl;
    outFile.close();
}

nlohmann::json JSON::GetAccountInformation(std::string account_id, JSON::File file) {
    json jsonData;
    std::ifstream inFile(file == JSON::File::ACCOUNTS ? "accounts.json" : "");
    if (!inFile.good()) { return nullptr; }
    try { inFile >> jsonData; }
    catch (const json::parse_error& e) { return nullptr; }
    if (!jsonData.contains(account_id) || jsonData[account_id].empty() || !jsonData[account_id].is_array()) { return nullptr; }
    const auto& entry = jsonData[account_id][0];
    if (!entry.contains("username") || !entry.contains("deviceId") || !entry.contains("secret")) { return nullptr; }
    return entry;
}

std::string JSON::GetAccountUsername(std::string account_id, JSON::File file) {
	return GetAccountInformation(account_id, file)["username"];
}

std::string JSON::GetIdFromUsername(std::string username, JSON::File file) {
    json jsonData;

    std::ifstream inFile(file == JSON::File::ACCOUNTS ? "accounts.json" : "");

    if (!inFile.good()) { return "nullptr"; }

    try { inFile >> jsonData; }

    catch (const json::parse_error& e) { return "nullptr"; }

    for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
        for (const auto& user : it.value()) {
            if (user["username"] == username) {
                return it.key();
                break;
            }
        }
    }
}

std::vector<std::string> JSON::GetAllUsers() {
    nlohmann::json j;
	std::ifstream inFile("accounts.json");
	if (!inFile.good()) {
		return {};
	}
	inFile >> j;
    std::vector<std::string> users;
    for (auto it = j.begin(); it != j.end(); ++it) {
        users.push_back(it.key());
    }
    return users;
}