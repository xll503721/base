#ifndef LB_BASE_TRACKER_PCH_H
#define LB_BASE_TRACKER_PCH_H

#include "../pch.h"
#include <network/http:s/http_s_client.h>
#include <storage/database/database.h>
#include <device/device.h>

BEGIN_NAMESPACE_BASE_ANALYTICS

class Tracker {

public:
    Tracker();
    static Tracker& DefaultTracker();
    
    void Send(const std::string& event_name, std::map<std::string, std::string>& event_properties);
    
private:
    BASE_HTTPS::HTTPsClient& https_client_;
    std::shared_ptr<BASE_STORAGE_DATABASE::DataBase> data_base_;
};

END_NAMESPACE_BASE_ANALYTICS

#endif /* LB_BASE_TRACKER_PCH_H */
