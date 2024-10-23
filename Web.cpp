#include "Web.h"
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <functional>

size_t Web::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string Web::webRequest(std::string url, std::string body, Web::RequestType type) {
    CURL* curl = curl_easy_init();
    std::string response;
    body == "" ? body = "{}" : 0;

    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Set request type
        type == Web::RequestType::POST && !body.empty() ? curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str()) : 0;
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
}

