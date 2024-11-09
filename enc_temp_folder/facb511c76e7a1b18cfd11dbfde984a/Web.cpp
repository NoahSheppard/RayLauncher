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

/*std::string Web::POSTRequest(const std::string url, const std::string body, const std::list<std::string> headers) {
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
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            response = "Error: " + std::string(curl_easy_strerror(res));
        }

        curl_slist_free_all(rheaders);
        curl_easy_cleanup(curl);
    }
    catch (const std::exception& e) { return "Error: Exception caught - " + std::string(e.what()); }

    return response;
}

std::string Web::GETRequest(const std::string url, const std::list<std::string> headers) {
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
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");


        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            response = "Error: " + std::string(curl_easy_strerror(res));
        }

        curl_slist_free_all(rheaders);
        curl_easy_cleanup(curl);
    }
    catch (const std::exception& e) { return "Error: Exception caught - " + std::string(e.what()); }

    return response;
}*/

std::string Web::POSTRequest(const std::string url, const std::string body, const std::list<std::string> headers) {
    return Web::Request(url, body, headers, Web::RequestType::POST);
}

std::string Web::GETRequest(const std::string url, const std::list<std::string> headers) {
	return Web::Request(url, "", headers, Web::RequestType::GET);
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

        if (type == Web::RequestType::POST) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
        }
        else if (type == Web::RequestType::GET) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        }
        else {
			return "Error: Invalid function call";
        }


        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            response = "Error: " + std::string(curl_easy_strerror(res));
        }

        curl_slist_free_all(rheaders);
        curl_easy_cleanup(curl);
    }
    catch (const std::exception& e) { return "Error: Exception caught - " + std::string(e.what()); }

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
		std::list<std::string> headers = { "Content-Type: application/x-www-form-urlencoded", "Authorization: Basic YWY0M2RjNzFkZDkxNDUyMzk2ZmNkZmZiZDdhOGU4YTk6NFlYdlNFQkxGUlBMaDFoekdaQWtmT2k1bXF1cEZvaFo=", "User-Agent: RayLauncher" };
		std::string body = "grant_type=device_auth&account_id=" + account_id + "&device_id=" + deviceId + "&secret=" + secret;
		std::string response = Web::POSTRequest("https://account-public-service-prod.ol.epicgames.com/account/api/oauth/token", body, headers);
		response.erase(std::remove(response.begin(), response.end(), '\\'), response.end());
		nlohmann::json jsonObj = nlohmann::json::parse(response);
        return jsonObj["access_token"];	
	}
    catch (const std::exception& e) {
        return "Error: Re-login, or open issue at github.com/NoahSheppard/RayLauncher";
    }
}

std::string Web::GetExchangeCodeWithBearer(std::string bearer_token) {
    try {
        std::list<std::string> headers = { "Authorization: bearer " + bearer_token};
		std::string response = Web::GETRequest("https://account-public-service-prod.ol.epicgames.com/account/api/oauth/exchange", headers); 
		response.find("Error") != std::string::npos ? throw std::exception() : 0;
        response.erase(std::remove(response.begin(), response.end(), '\\'), response.end());
        return nlohmann::json::parse(response)["code"];
    }
    catch (const std::exception& e) { return "Error: Re-login, or open issue at github.com/NoahSheppard/RayLauncher"; }
}