#include <string>
#include "nlohmann/json.hpp"

class JSON {
public:
    enum File
    {
        ACCOUNTS
    };
	static std::string DefaultAccountFile;
    static void AddAccountToFile(std::string account_id, std::string username, std::string deviceId, std::string secret, JSON::File file);
    static nlohmann::json GetAccountInformation(std::string username, JSON::File file);

};