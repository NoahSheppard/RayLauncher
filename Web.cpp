#include "Web.h"
#include "JSON.h"
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <functional>
#include "json.hpp"
#include <algorithm>
#include <wx/wx.h>


size_t Web::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    try {
        userp->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    catch (const std::exception& e) {
        return 0;  // Indicate error
    }
}

std::string Web::Request(const std::string url, const std::string body, const std::list<std::string> headers, Web::RequestType type) {
    std::string response;
    CURL* curl = curl_easy_init();
    if (!curl) { return "Error: Failed to initialize CURL"; }

    try {
        struct curl_slist* rheaders = nullptr;
        for (std::string header : headers) {
            rheaders = curl_slist_append(rheaders, header.c_str());
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, rheaders);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Make sure we're setting the correct HTTP method
        if (type == Web::RequestType::POST) {
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            if (!body.empty()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            }
        }
        else if (type == Web::RequestType::GET) {
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        }

        // Store request details for debugging
        std::string debug_info = "URL: " + url + "\n\n";
        debug_info += "Request Type: " + std::string(type == Web::RequestType::POST ? "POST" : "GET") + "\n";
        debug_info += "Headers:\n";
        for (const auto& header : headers) {
            debug_info += header + "\n";
        }
        if (!body.empty()) {
            debug_info += "\nBody: " + body + "\n";
        }

        CURLcode res = curl_easy_perform(curl);
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        debug_info += "\nResponse Code: " + std::to_string(http_code) + "\n";
        debug_info += "Response: " + response + "\n";
        if (res != CURLE_OK) {
            debug_info += "CURL Error: " + std::string(curl_easy_strerror(res)) + "\n";
        }

        JSON::WriteDebugToFile("C:\\Users\\Noah\\Desktop\\request_debug.txt", debug_info);

        curl_slist_free_all(rheaders);
        curl_easy_cleanup(curl);
    }
    catch (const std::exception& e) {
        return "Error: Exception caught - " + std::string(e.what());
    }

    return response;
}

std::string Web::POSTRequest(const std::string url, const std::string body, const std::list<std::string> headers) {
    return Web::Request(url, body, headers, Web::RequestType::POST);
}

std::string Web::GETRequest(const std::string url, const std::list<std::string> headers) {
    // Log the request details before making it
    std::string debug_info = "GET Request Details:\n";
    debug_info += "URL: " + url + "\n\n";
    debug_info += "Headers:\n";
    for (const auto& header : headers) {
        debug_info += header + "\n";
    }
    JSON::WriteDebugToFile("C:\\Users\\Noah\\Desktop\\pre_request_debug.txt", debug_info);

    std::string response = Web::Request(url, "", headers, Web::RequestType::GET);

    // Log the response
    debug_info = "GET Response:\n" + response + "\n";
    JSON::WriteDebugToFile("C:\\Users\\Noah\\Desktop\\post_request_debug.txt", debug_info);

    return response;
}

std::string Web::Login(std::string code) {
    try {
        std::list<std::string> headers = { "Content-Type: application/x-www-form-urlencoded", "Authorization: Basic YWY0M2RjNzFkZDkxNDUyMzk2ZmNkZmZiZDdhOGU4YTk6NFlYdlNFQkxGUlBMaDFoekdaQWtmT2k1bXF1cEZvaFo=", "User-Agent: RayLauncher" };
        code = "grant_type=authorization_code&code=" + code;
        std::string response = Web::POSTRequest("https://account-public-service-prod.ol.epicgames.com/account/api/oauth/token", code, headers);
        response.erase(std::remove(response.begin(), response.end(), '\\'), response.end());
        nlohmann::json jsonObj = nlohmann::json::parse(response);
        std::string displayName = (std::string)jsonObj["displayName"];

        headers = { "Content-Type: application/json", "Authorization: Bearer " + (std::string)jsonObj["access_token"], "User-Agent: RayLauncher" };
        response = Web::POSTRequest("https://account-public-service-prod.ol.epicgames.com/account/api/public/account/" + (std::string)jsonObj["account_id"] + "/deviceAuth", "", headers);
        response.erase(std::remove(response.begin(), response.end(), '\\'), response.end());
        jsonObj = nlohmann::json::parse(response);
        JSON::AddAccountToFile((std::string)jsonObj["accountId"], displayName, (std::string)jsonObj["deviceId"], (std::string)jsonObj["secret"], JSON::File::ACCOUNTS);
        return "";
    }
    catch (const std::exception& e) {
        return "Error: Re-login, or open issue at github.com/NoahSheppard/RayLauncher";
    }
}

std::string Web::GetToken(std::string account_id) {
    try {
        std::string username = JSON::GetAccountUsername(account_id, JSON::File::ACCOUNTS);
        std::string deviceId = JSON::GetAccountInformation(account_id, JSON::File::ACCOUNTS)["deviceId"];
        std::string secret = JSON::GetAccountInformation(account_id, JSON::File::ACCOUNTS)["secret"];

        // Debug the input values
        std::string debug_info = "Account ID: " + account_id + "\n";
        debug_info += "Username: " + username + "\n";
        debug_info += "Device ID: " + deviceId + "\n";
        debug_info += "Secret length: " + std::to_string(secret.length()) + "\n";

        std::list<std::string> headers = {
            "Content-Type: application/x-www-form-urlencoded",
            "Authorization: Basic YWY0M2RjNzFkZDkxNDUyMzk2ZmNkZmZiZDdhOGU4YTk6NFlYdlNFQkxGUlBMaDFoekdaQWtmT2k1bXF1cEZvaFo=",
            "User-Agent: RayLauncher",
            "Accept: */*"  // Added Accept header
        };

        std::string body = "grant_type=device_auth&account_id=" + account_id + "&device_id=" + deviceId + "&secret=" + secret;
        debug_info += "Request Body: " + body + "\n";

        // Log headers
        debug_info += "\nHeaders:\n";
        for (const auto& header : headers) {
            debug_info += header + "\n";
        }

        std::string response = Web::POSTRequest("https://account-public-service-prod.ol.epicgames.com/account/api/oauth/token", body, headers);
        debug_info += "\nRaw Response: " + response + "\n";

        // Remove backslashes from response
        response.erase(std::remove(response.begin(), response.end(), '\\'), response.end());
        debug_info += "Cleaned Response: " + response + "\n";

        JSON::WriteDebugToFile("C:\\Users\\Noah\\Desktop\\token_debug.txt", debug_info);

        try {
            nlohmann::json jsonObj = nlohmann::json::parse(response);
            if (jsonObj.contains("access_token")) {
                return jsonObj["access_token"];
            }
            else {
                return "Error: No access token in response - " + response;
            }
        }
        catch (const nlohmann::json::parse_error& e) {
            return "Error: JSON parse failed - " + std::string(e.what()) + " - Raw response: " + response;
        }
    }
    catch (const std::exception& e) {
        return "Error: Exception caught - " + std::string(e.what());
    }
}

std::string Web::GetExchangeCodeWithBearer(std::string bearer_token) {
    try {
        std::list<std::string> headers = { "Authorization: bearer " + bearer_token };
        std::string response = Web::GETRequest("https://account-public-service-prod.ol.epicgames.com/account/api/oauth/exchange", headers);
        response.find("Error") != std::string::npos ? throw std::exception() : 0;
        response.erase(std::remove(response.begin(), response.end(), '\\'), response.end());
        return nlohmann::json::parse(response)["code"];
    }
    catch (const std::exception& e) { return "Error: Re-login, or open issue at github.com/NoahSheppard/RayLauncher"; }
}