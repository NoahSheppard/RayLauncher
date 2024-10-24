#include "Web.h"
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <functional>


size_t Web::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    try {
        userp->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    catch (const std::exception& e) {
        return 0;  // Indicate error
    }
}

/*std::string Web::webRequest(std::string url, std::string body, Web::RequestType type) {
    CURL* curl = curl_easy_init();
    std::string response;
    body == "" ? body = "{}" : 0;

    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        //headers = curl_slist_append(headers, "Authorization: Basic YWY0M2RjNzFkZDkxNDUyMzk2ZmNkZmZiZDdhOGU4YTk6NFlYdlNFQkxGUlBMaDFoekdaQWtmT2k1bXF1cEZvaFo=");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Set request type
        std::string postFields = "grant_type=authorization_code&code=" + body;
        type == Web::RequestType::POST && !body.empty() ? curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str()) : 0;
        type == Web::RequestType::POST ? curl_easy_setopt(curl, CURLOPT_POST, 1L) : 0;
        type == Web::RequestType::GET ? curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L) : 0;

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            response = "Error: " + std::string(curl_easy_strerror(res));
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    else {
        response = "Error: Failed to initialize CURL";
    }

    return response;
}*/

/*std::string Web::webRequest(const std::string url, const std::string body, Web::RequestType type) {
    // Initialize response string
    std::string response;

    // Initialize CURL
    CURL* curl = curl_easy_init();
    if (!curl) {
        return "Error: Failed to initialize CURL";
    }

    try {
        // Initialize headers
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, "Authorization: Basic YWY0M2RjNzFkZDkxNDUyMzk2ZmNkZmZiZDdhOGU4YTk6NFlYdlNFQkxGUlBMaDFoekdaQWtmT2k1bXF1cEZvaFo=");

        // Copy URL to ensure it's null-terminated and valid
        std::string safeUrl = url;

        // Set CURL options with error checking
        if (curl_easy_setopt(curl, CURLOPT_URL, safeUrl.c_str()) != CURLE_OK ||
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers) != CURLE_OK ||
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback) != CURLE_OK ||
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response) != CURLE_OK) {

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            return "Error: Failed to set CURL options";
        }

        // Handle request type
        if (type == Web::RequestType::POST) {
            // Format the POST fields correctly
            std::string postFields = "grant_type=authorization_code&code=" + body;
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1L);

            // Debug: Print the post fields
            //printf("Post Fields: %s\n", postFields.c_str());
        }
        else if (type == Web::RequestType::GET) {
            if (curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L) != CURLE_OK) {
                curl_slist_free_all(headers);
                curl_easy_cleanup(curl);
                return "Error: Failed to set GET options";
            }
        }

        // Set additional safety options
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);  // 30 second timeout
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        // Perform request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            response = "Error: " + std::string(curl_easy_strerror(res));
        }

        // Get HTTP response code
        long http_code = 0;
        /*curl_easy_getopt(curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code != 200) {
            response += "\nHTTP Error Code: " + std::to_string(http_code);
        }

        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    catch (const std::exception& e) {
        return "Error: Exception caught - " + std::string(e.what());
    }

    return response;
}*/

std::string Web::webRequest(const std::string url, const std::string body, Web::RequestType type) {
    // make headers input a list of string which are meant to be headers
    // for instance, take in
    // std::list<std::string> {"Content-Type: application/x-www-form-urlencoded", "Authorization: Basic YWY0M2RjNzFkZDkxNDUyMzk2ZmNkZmZiZDdhOGU4YTk6NFlYdlNFQkxGUlBMaDFoekdaQWtmT2k1bXF1cEZvaFo=", "User-Agent: RayLauncher"}
    // and then
    // struct curl_slist* headers = nullptr;
    // for (header : headers) {
    //     headers = curl_stlis_append(headers, header)
    // }
    std::string response;
    CURL* curl = curl_easy_init();
    //body = "grant_type=authorization_code&code=" + body;
    if (!curl) { return "Error: Failed to initialize CURL"; }

    try {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, "Authorization: Basic YWY0M2RjNzFkZDkxNDUyMzk2ZmNkZmZiZDdhOGU4YTk6NFlYdlNFQkxGUlBMaDFoekdaQWtmT2k1bXF1cEZvaFo=");
        headers = curl_slist_append(headers, "User-Agent: RayLauncher");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        if (type == Web::RequestType::POST) {
            // Format body exactly like the JavaScript URLSearchParams
            //std::string postFields = "grant_type=authorization_code&code=" + body;
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
        }

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            response = "Error: " + std::string(curl_easy_strerror(res));
        }

        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

    }
    catch (const std::exception& e) {
        return "Error: Exception caught - " + std::string(e.what());
    }

    return response;
}

std::string Web::POSTRequest(const std::string url, const std::string body, const std::list<std::string> headers) {
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

std::string Web::GetInitialInfo(std::string code) {
    //std::string code = "grant_type=authorization_code&code=1e0d3d4398f048dbb70a36ef9e159358";
    //std::string firstRequest = Web::webRequest("https://account-public-service-prod.ol.epicgames.com/account/api/oauth/token", code, Web::RequestType::POST);
    //wxLogStatus("Request: " + (wxString)firstRequest);
    //JSON::AddAccountToFile("Error", firstRequest, "", "", JSON::File::ACCOUNTS);
    std::list<std::string> headers = { "Content-Type: application/x-www-form-urlencoded", "Authorization: Basic YWY0M2RjNzFkZDkxNDUyMzk2ZmNkZmZiZDdhOGU4YTk6NFlYdlNFQkxGUlBMaDFoekdaQWtmT2k1bXF1cEZvaFo=", "User-Agent: RayLauncher"};
    code = "grant_type=authorization_code&code=" + code;
    std::string response = Web::POSTRequest("https://account-public-service-prod.ol.epicgames.com/account/api/oauth/token", code, headers);
    return response;
}