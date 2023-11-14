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
config_(config) {
    config_ = config;
}

void HTTPsClient::Get(const std::string& url_string, RequestCompletion requestCompletion) {
    if (url_string.empty()) {
        otlog_fault << "url_string must be not empty";
        return;
    }
    otlog_info << "request url string:" << config_.host << url_string.c_str();
    
    int32_t status = 200;
    std::string body = "";
    requestCompletion(this, static_cast<HTTPsClient::Status>(status), body);
    
//    BASE_THREAD::ThreadPool::DefaultPool().ScheduleTransfer(BASE_THREAD::Thread::Type::kHTTP, [=](){
//        httplib::Client cli(config_.host);
//
//        auto res = cli.Get(url_string);
//        if (!res) {
//            otlog_fault << "request failed";
//            BASE_THREAD::ThreadPool::DefaultPool().ScheduleTransferBack([=](){
//                requestCompletion(this, HTTPsClient::Status::kStatus500, "");
//            });
//            return;
//        }
//        auto status = res->status;
//        auto body = res->body;
//        
//        int32_t status = 200;
//        std::string body = "";
//        
//        otlog_info << "response url string:" << url_string.c_str() << ", body:" << body.c_str();
//        BASE_THREAD::ThreadPool::DefaultPool().ScheduleTransferBack([=](){
//            requestCompletion(this, static_cast<HTTPsClient::Status>(status), body);
//        });
//    });
}

void HTTPsClient::Post(const std::string& url_string, std::map<std::string, std::string> params, RequestCompletion requestCompletion) {
    otlog_info << "request url string:" << url_string.c_str();
    
    int32_t status = 200;
    std::string body = "";
    requestCompletion(this, static_cast<HTTPsClient::Status>(status), body);
//    BASE_THREAD::ThreadPool::DefaultPool().ScheduleTransfer(BASE_THREAD::Thread::Type::kHTTP, [=](){
//        httplib::Client cli(config_.host);
//
//        httplib::Params params;
//        for (auto it = params.begin(); it != params.end(); it++) {
//            params.emplace(it->first, it->second);
//        }
//        
//        auto res = cli.Post(url_string, params);
//        auto status = res->status;
//        auto body = res->body;
//        
//        int32_t status = 200;
//        std::string body = "";
//        
//        otlog_info << "response url string:" << url_string.c_str() << ", body:" << body.c_str();
//        BASE_THREAD::ThreadPool::DefaultPool().ScheduleTransferBack([=](){
//            requestCompletion(this, static_cast<HTTPsClient::Status>(status), body);
//        });
//    });
}

END_NAMESPACE_BASE_HTTPS
