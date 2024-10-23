#include <string>

class Web {
public:
    static enum RequestType
    {
        POST = 1, GET = 2
    };
    static std::string webRequest(std::string url, std::string body, Web::RequestType type);
private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
};