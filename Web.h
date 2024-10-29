#include <string>
#include <list>

class Web {
public:
    static enum RequestType
    {
        POST = 1, GET = 2
    };
    static std::string POSTRequest(const std::string url, const std::string body, const std::list<std::string> headers);
    static std::string Login(std::string code);
private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
};