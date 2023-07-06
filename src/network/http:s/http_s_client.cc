//
//  http_s_client.cpp
//  base_https
//
//  Created by li zhixuan on 2023/7/6.
//

#include "http_s_client.h"
#include "httplib.h"

BEGIN_NAMESPACE_BASE_HTTPS

void HTTPsClient::Test() {
    // HTTPS
    httplib::Client cli("http://cpp-httplib-server.yhirose.repl.co");

    auto res = cli.Get("/hi");
    res->status;
    auto body = res->body;
    printf("%s", body.c_str());
}

END_NAMESPACE_BASE_HTTPS
