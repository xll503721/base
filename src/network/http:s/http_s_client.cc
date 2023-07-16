//
//  http_s_client.cpp
//  base_https
//
//  Created by li zhixuan on 2023/7/6.
//

#include "http_s_client.h"
#include "httplib.h"

BEGIN_NAMESPACE_BASE_HTTPS

HTTPsClient& HTTPsClient::DefaultClient(const std::string& host) {
    Configuration config;
    config.timeout = 3000;
    config.host = host;
    
    static HTTPsClient https_client(config);
    return https_client;
}

HTTPsClient::HTTPsClient(const Configuration& config):
thread_pool_(1, BASE_THREAD::Thread::Type::kHTTP),
config_(config) {
    config_ = config;
}

void HTTPsClient::Get(const std::string& url_string, RequestCompletion requestCompletion) {
//    thread_pool_.ScheduleTransfer(BASE_THREAD::Thread::Type::kHTTP, [=](){
//        httplib::Client cli(config_.host);
//
//        auto res = cli.Get(url_string);
//        auto status = res->status;
//        auto body = res->body;
//        
//        thread_pool_.ScheduleTransferBack(BASE_THREAD::Thread::Type::kHTTP, [=](){
//            requestCompletion(this, static_cast<HTTPsClient::Status>(status), body);
//        });
//    });
}

END_NAMESPACE_BASE_HTTPS
