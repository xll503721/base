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
Platform::PlatformSetDelegate Platform::set_delegate_fun_ = nullptr;

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

Platform::Var Platform::GetVar(Var var) {
    return var;
}

void Platform::SetInitMehtod(PlatformInit method) {
    init_fun_ = method;
}

void Platform::SetPerformMehtod(PlatformPerform method) {
    perform_fun_ = method;
}

void Platform::SetPerformDelegate(PlatformSetDelegate method) {
    set_delegate_fun_ = method;
}

void Platform::Init(const std::string& class_name, std::shared_ptr<void> c_plus_plus_obj) {
    if (!init_fun_) {
        return nullptr;
    }
    
    platform_obj_ = init_fun_(class_name, c_plus_plus_obj);
    if (!platform_obj_) {
        otlog_fault << "platform_obj create failed, check class_name:" << class_name << ", c_plus_plus_obj:" << c_plus_plus_obj;
    }
}

void Platform::SetDelegate(std::shared_ptr<void> delegate, const std::string& file_name) {
    if (!set_delegate_fun_) {
        return;
    }
    
    c_plus_plus_obj_ = delegate;
    auto file_name_string = file_name;
    BASE_STRING::ReplaceAll(file_name_string, ".cc", "");
    BASE_STRING::ReplaceAll(file_name_string, ".h", "");
    set_delegate_fun_(platform_obj_, file_name_string, delegate);
}

std::shared_ptr<Platform::Var> Platform::Perform(const std::string& file_name, const std::string& method_name_full, bool is_set_delegate, const std::string& params_name, BASE_PLATFORM::Platform::Var* params, ...) {
    std::shared_ptr<Platform::Var> error_var;
    
    if (!platform_obj_ || !perform_fun_) {
        return error_var;
    }
    
    std::vector<BASE_PLATFORM::Platform::Var*> params_vector;
    if (params) {
        params_vector.push_back(params);
        va_list args;
        va_start(args, params);
        BASE_PLATFORM::Platform::Var* arg;
        while ((arg = va_arg(args, BASE_PLATFORM::Platform::Var*))) {
            params_vector.push_back((arg));
        }
        va_end(args);
    }
    
    std::vector<std::string> params_name_vector;
    std::string file_name_string;
    std::string return_type;
    std::string only_method;
    do {
        if (method_name_full_and_cache_map_.find(method_name_full) != method_name_full_and_cache_map_.end()) {
            auto cache = method_name_full_and_cache_map_[method_name_full];
            
            params_name_vector = cache.params_name_vector;
            file_name_string = cache.file_name;
            return_type = cache.return_type;
            only_method = cache.method;
            break;
        }

        auto params_name_string = params_name;
        BASE_STRING::ReplaceAll(params_name_string, "&", "");
        BASE_STRING::ReplaceAll(params_name_string, " ", "");
        params_name_vector = BASE_STRING::Split(params_name_string, ",");
        
        file_name_string = file_name;
        BASE_STRING::ReplaceAll(file_name_string, ".cc", "");
        
        if (!ParseMethod(method_name_full, return_type, only_method)) {
            return error_var;
        }
        
        if (isPlatform(PlatformType::kPlatformTypeiOS)) {
            ParseMethodStyleiOS(params_name_vector, only_method);
        }
        
//        if (method_name_full_and_cache_map_.find(method_name_full) != method_name_full_and_cache_map_.end()) {
//            PlatformMethodCache cache;
//            cache.method = only_method;
//            cache.return_type = return_type;
//            cache.params_name_vector = params_name_vector;
//            cache.file_name = file_name;
//            
//            method_name_full_and_cache_map_[method_name_full] = cache;
//        }
    } while (false);
    
//    otlog_info << "platform obj:" << platform_obj_ << ", file name string:" << file_name_string
//    << ", only method:" << only_method << ", is set delegate:" << is_set_delegate; /*<< ", params name vector:" << params_name_vector << ", params vector:" << params_vector;*/
    return perform_fun_(platform_obj_, file_name_string, only_method, is_set_delegate, params_name_vector, params_vector);
}

void Platform::ParseMethodStyleiOS(const std::vector<std::string>& params_name_vector, std::string& only_method) {
    std::vector<std::string> upper_params_name_word;
    for (int i = 0; i < params_name_vector.size(); i++) {
        if (i == 0) {
            only_method += "With";
        }
        
        auto params_name = params_name_vector[i];
        std::vector<std::string> params_name_word = BASE_STRING::Split(params_name, "_");
        
        std::string full_params_name;
        for (int j = 0; j < params_name_word.size(); j++) {
            auto one_word = params_name_word[j];
            if (i == 0 || j != 0) {
                one_word[0] = std::toupper(one_word[0]);
                BASE_STRING::ReplaceAll(one_word, "Get", "");
            }
            full_params_name += one_word;
            if (j == params_name_word.size() - 1) {
                full_params_name += ":";
            }
        }
        only_method += full_params_name;
    }
}

bool Platform::ParseMethod(const std::string& method_name_full, std::string& retrun_type, std::string& only_method) {
    //parse mehtod
    std::vector<std::string> method_name_full_vector = BASE_STRING::Split(method_name_full, " ");
    if (method_name_full_vector.size() <= 1) {
        otlog_fault << "check invoke method name";
        return false;
    }
    //return type, such as 'std::string'
    std::string return_type = method_name_full_vector[0];
    //method_params, such as 'ot_base_device::Device::File::GetCachesPath()'
    std::string method_params = method_name_full_vector[1];
    size_t index = method_params.find("(");
    if (index == std::string::npos) {
        otlog_fault << "check invoke method name";
        return false;
    }
    //method, such as ot_base_device::Device::File::GetCachesPath
    std::string method = method_params.substr(0, index);
    
    std::string find_str(":");
    size_t colon_index = method.rfind(find_str);
    //method, such as GetCachesPath
    only_method = method.substr(colon_index + find_str.size());
    
    std::string get("Get");
    if (BASE_STRING::StartsWith(only_method, get)) {
        only_method.erase(0, get.size());
    }
    only_method[0] = tolower(only_method[0]);
    return true;
}

void* Platform::GetPlatformObj() {
    return platform_obj_;
}

END_NAMESPACE_BASE_PLATFORM
