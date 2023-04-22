//
//  oneten_ad_sdk.cpp
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "oneten_ad_sdk.h"
#include "loader/main_loader.h"
#include "loader/waterfall_loader.h"
#include "loader/header_bid_loader.h"
#include "loader/placement_loader.h"
#include "loader/normal_loader.h"
#include "loader/cache_loader.h"

BEGIN_NAMESPACE_ONETEN_AD

//thread_local AdSDKDelegate* delegate_ = nullptr;

OnetenAdSDK &OnetenAdSDK::GetInstance() {
    static OnetenAdSDK ad_sdk;
    return ad_sdk;
}

OnetenAdSDK::OnetenAdSDK() {
}

OnetenAdSDK::~OnetenAdSDK() {
}

void OnetenAdSDK::Register(const std::string& app_id) {
//    printf("%s", app_id.c_str());
}

std::shared_ptr<LoaderInterface> OnetenAdSDK::GetMainLoader() {
    return start_loader_;
}

std::shared_ptr<LoaderInterface> OnetenAdSDK::GetWaterfallLoader() {
    return waterfall_loader_;
}

std::shared_ptr<LoaderInterface> OnetenAdSDK::GetCacheLoader() {
    return cache_loader_;
}

std::shared_ptr<LoaderInterface> OnetenAdSDK::GetRequestLoader() {
    return request_loader_;
}

void OnetenAdSDK::StartAdLoad(const std::string& placement_id, std::map<std::string, std::string>& user_info, AdSDKDelegate& delegate) {
    delegate_ = &delegate;
    user_info_ = user_info;
    
    std::shared_ptr<MainLoader> start_main_loader = std::make_shared<MainLoader>(nullptr);
    std::shared_ptr<PlacementLoader> placement_loader = std::make_shared<PlacementLoader>(start_main_loader);
    
    std::shared_ptr<MainLoader> waterfall_main_loader = std::make_shared<MainLoader>(nullptr);
    std::shared_ptr<WaterfallLoader> waterfall_loader = std::make_shared<WaterfallLoader>(waterfall_main_loader);
    
    std::shared_ptr<MainLoader> request_main_loader = std::make_shared<MainLoader>(nullptr);
    std::shared_ptr<HeaderBidLoader> header_bid_loader = std::make_shared<HeaderBidLoader>(request_main_loader);
    std::shared_ptr<NormalLoader> normal_loader = std::make_shared<NormalLoader>(header_bid_loader);
    
    std::shared_ptr<MainLoader> save_main_loader = std::make_shared<MainLoader>(nullptr);
    std::shared_ptr<CacheLoader> cache_loader = std::make_shared<CacheLoader>(save_main_loader);
    
    start_loader_ = placement_loader;
    waterfall_loader_ = waterfall_loader;
    request_loader_ = normal_loader;
    cache_loader_ = cache_loader;
    
    start_loader_->Start(placement_id);
}

void OnetenAdSDK::EndAdLoad(const std::string& placement_id) {
    start_loader_->End();
    waterfall_loader_->End();
    request_loader_->End();
    cache_loader_->End();
    
    if (delegate_) {
        delegate_->LoadSucceed();
    }
}

bool OnetenAdSDK::IsReady(const std::string& placement_id) {
    
}

void OnetenAdSDK::ShowAd(const std::string& placement_id, AdSDKDelegate& delegate) {
    
}

END_NAMESPACE_ONETEN_AD
