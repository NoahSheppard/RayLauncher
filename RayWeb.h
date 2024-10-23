#pragma once
#include <string>
class RayWeb
{
public:
	static std::string POST(std::string url, std::string body, std::string content_type = "Content-Type: application/x-www-form-urlencoded");
	static std::string urlEncode(const std::string& value);
private:
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
};

