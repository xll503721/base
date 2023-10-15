//
//  http_s_client.hpp
//  base_https
//
//  Created by li zhixuan on 2023/7/6.
//

#ifndef LB_BASE_NETWORK_HTTPS_CLIENT_H
#define LB_BASE_NETWORK_HTTPS_CLIENT_H

#include "../../pch.h"

#include <thread/thread_pool.h>

BEGIN_NAMESPACE_BASE_HTTPS

class HTTPsClient {
    
public:
    
    enum class Status {
        kStatus200 = 200,
        kStatus500 = 500,
        kStatus404 = 404,
    };
    
    struct Configuration {
        uint32_t timeout;
        std::string host;
    };
    
    static HTTPsClient& DefaultClient(const std::string& host);
    
    HTTPsClient(const Configuration& config);
    
    using RequestCompletion = std::function<void (HTTPsClient* client, HTTPsClient::Status status, const std::string& body)>;
    
    void Get(const std::string& url_string, RequestCompletion requestCompletion);
    void Post(const std::string& url_string, std::map<std::string, std::string> parmas, RequestCompletion requestCompletion);
    
private:
    Configuration config_;
};

END_NAMESPACE_BASE_HTTPS

#endif /* LB_BASE_NETWORK_HTTPS_CLIENT_H */
