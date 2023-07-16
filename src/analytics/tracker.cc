//
//  MmapFile.cc
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "tracker.h"

BEGIN_NAMESPACE_BASE_ANALYTICS

static const std::string host = "tracker_host";
static const std::string path = "tracker_path";
static const std::string database_name = "tracker_database_name";

Tracker& Tracker::DefaultTracker() {
    static Tracker tracker;
    return tracker;
}

Tracker::Tracker():
https_client_(BASE_HTTPS::HTTPsClient::DefaultClient(host)) {
    data_base_ = std::make_shared<BASE_STORAGE_DATABASE::DataBase>(BASE_DEVICE::Device::DefaultInstance().GetFile()->GetCachesPath() + "/" + database_name);
    const char* sql_create_table =
            "CREATE TABLE IF NOT EXISTS Users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT,"
            "age INTEGER);";
    data_base_->CreateTable(sql_create_table);
}

void Tracker::Send(const std::string& event_name, std::map<std::string, std::string>& event_properties) {
    std::map<std::string, void*> user_info;
//    user_info["event_name"] = static_cast<void*>(event_name);
//    user_info["event_properties"] = static_cast<void*>(event_properties);
    https_client_.Get(path, [=](BASE_HTTPS::HTTPsClient* client, BASE_HTTPS::HTTPsClient::Status status, const std::string& body){
        if (status != BASE_HTTPS::HTTPsClient::Status::kStatus200) {
            
        }
    });
}

END_NAMESPACE_BASE_ANALYTICS
