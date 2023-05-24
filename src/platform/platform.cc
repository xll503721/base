//
//  platform.cc
//  OneTenSDK
//
//  Created by li zhixuan on 2023/4/9.
//

#include "platform.h"
#include <string/string.h>
#include <storage/memory/storage_memory.h>

BEGIN_NAMESPACE_BASE_PLATFORM

Platform::PlatformInit Platform::init_fun_ = nullptr;
Platform::PlatformPerform Platform::perform_fun_ = nullptr;

Platform::Platform(const std::string& class_name, std::shared_ptr<void> c_plus_plus_obj):
platform_obj_(nullptr),
c_plus_plus_obj_(c_plus_plus_obj) {
}

bool Platform::isPlatform(PlatformType type) {
    bool is_platform = false;
    switch (type) {
#ifdef OS_IOS
        case PlatformType::kPlatformTypeiOS: {
            is_platform = true;
        }
            break;
#endif
#ifdef OS_MAX
        case PlatformType::kPlatformTypeMac: {

            is_platform = true;
        }
            break;
#endif
#ifdef OS_ANDROID
        case PlatformType::kPlatformTypeAndroid: {
            is_platform = true;
        }
            break;
#endif
            
        default:
            break;
    }
    return is_platform;
}

Platform::PlatformType Platform::platform() {
#ifdef OS_IOS
    return PlatformType::kPlatformTypeiOS;
#endif
#ifdef OS_MAX
    return PlatformType::kPlatformTypeMac;
#endif
#ifdef OS_ANDROID
    return PlatformType::kPlatformTypeAndroid;
#endif
}

void Platform::SetInitMehtod(PlatformInit method) {
    init_fun_ = method;
}

void Platform::SetPerformMehtod(PlatformPerform method) {
    perform_fun_ = method;
}

void Platform::Init(const std::string& file_name, const std::string& class_name, void* c_plus_plus_obj) {
    if (!init_fun_) {
        return nullptr;
    }
    
    auto file_name_string = file_name;
    BASE_STRING::ReplaceAll(file_name_string, ".cc", "");
    
    platform_obj_ = init_fun_(file_name_string, class_name, c_plus_plus_obj);
    if (!platform_obj_) {
        
    }
}

void* Platform::Perform(const std::string& file_name, const std::string& method_name, bool is_set_delegate, const std::string& params_name, BASE_PLATFORM::Platform::Var* params, ...) {
    if (!platform_obj_) {
        return;
    }

    std::vector<BASE_PLATFORM::Platform::Var*> params_vector;
    params_vector.push_back(params);
    
    va_list args;
    va_start(args, params);
    BASE_PLATFORM::Platform::Var* arg;
    while ((arg = va_arg(args, BASE_PLATFORM::Platform::Var*))) {
        params_vector.push_back((arg));
    }
    va_end(args);

    auto params_name_string = params_name;
    BASE_STRING::ReplaceAll(params_name_string, "&", "");
    BASE_STRING::ReplaceAll(params_name_string, " ", "");
    std::vector<std::string> params_name_vector = BASE_STRING::Split(params_name_string, ",");
    
    auto file_name_string = file_name;
    BASE_STRING::ReplaceAll(file_name_string, ".cc", "");
    
    auto method_name_string = method_name;
    method_name_string[0] = tolower(method_name_string[0]);
    std::string full_method_name_string = method_name_string;
    if (isPlatform(PlatformType::kPlatformTypeiOS)) {
        full_method_name_string += "With";
        
        std::vector<std::string> upper_params_name_word;
        for (int i = 0; i < params_name_vector.size(); i++) {
            auto params_name = params_name_vector[i];
            std::vector<std::string> params_name_word = BASE_STRING::Split(params_name, "_");
            
            std::string full_params_name;
            for (int j = 0; j < params_name_word.size(); j++) {
                auto one_word = params_name_word[j];
                if (i == 0 || j != 0) {
                    one_word[0] = std::toupper(one_word[0]);
                }
                full_params_name += one_word;
                if (j == params_name_word.size() - 1) {
                    full_params_name += ":";
                }
            }
            full_method_name_string += full_params_name;
        }
    }
    
    return perform_fun_(platform_obj_, file_name_string, full_method_name_string, is_set_delegate, params_name_vector, params_vector);
}

void* Platform::GetPlatformObj() {
    return platform_obj_;
}

END_NAMESPACE_BASE_PLATFORM
