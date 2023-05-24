//
//  platform.h
//  OneTenSDK
//
//  Created by li zhixuan on 2023/4/9.
//

#include "../pch.h"

#ifndef LB_BASE_PLATFORM_PCH_H
#define LB_BASE_PLATFORM_PCH_H

BEGIN_NAMESPACE_BASE_PLATFORM

#define PLATFORM_DECLARE \
std::shared_ptr<BASE_PLATFORM::Platform> platform_;\

#define PLATFORM_INIT(class_name, c_object) \
platform_ = std::make_shared<BASE_PLATFORM::Platform>(class_name, c_object);\

#define PLATFORM_INIT_DELEGATE(class_name, c_object) \
platform_ = std::make_shared<BASE_PLATFORM::Platform>(class_name, c_object);\
std::string delegate_file(__FILE_NAME__);\
platform_->Init(delegate_file, class_name, c_object.get());

#define PLATFORM_PERFORM(...) \
std::string parmas_str = #__VA_ARGS__;\
std::string file(__FILE_NAME__);\
std::string func(__FUNCTION__);\
void* PLATFORM_PERFORM_RESULT = platform_->Perform(file, func, false, parmas_str, ##__VA_ARGS__, nullptr);\

#define GET_PLATFORM_PERFORM_RESULT PLATFORM_PERFORM_RESULT

class Platform {
    
public:
    enum class PlatformType {
        kPlatformTypeiOS,
        kPlatformTypeMac,
        kPlatformTypeAndroid,
    };
    
    class Var {
    public:
        enum class Type {
            kTypeInt,
            kTypeBool,
            kTypeLong,
            kTypeFloat,
            kTypeDouble,
            kTypeString,
            kTypeMap,
            kTypeVector
        };
        Var() = default;
        Var(int32_t value) {
            value_.int32_ = value;
            type_ = Type::kTypeInt;
        }
        
        Var(double value) {
            value_.double_ = value;
            type_ = Type::kTypeDouble;
        }
        
        Var(std::unordered_map<std::string, Var>* value) {
            value_.map_ = value;
            type_ = Type::kTypeMap;
        }
        
        Var(std::vector<Var>* value) {
            value_.vector_ = value;
            type_ = Type::kTypeVector;
        }
        
        Type GetType() {
            return type_;
        }
        
        int32_t GetDataInt32() {
            return value_.int32_;
        }
        
        std::unordered_map<std::string, Var>* GetDataMap() {
            return value_.map_;
        }
        
    private:
        union ValueHolder {
            int64_t int64_;
            int32_t int32_;
            bool boolean_;
            double double_;
            float float_;
            void* ptr_;
            std::unordered_map<std::string, Var>* map_;
            std::vector<Var>* vector_;
        } value_;
        Type type_;
    };
    
    Platform() = default;
    Platform(const std::string& class_name, std::shared_ptr<void> c_plus_plus_obj);
    
    using PlatformInit = std::function<void* (const std::string& file_name, const std::string& class_name, void* c_plus_plus_obj)>;
    using PlatformPerform = std::function<void* (const void* platform_obj, const std::string& file_name, const std::string& method_name, bool is_set_delegate, const std::vector<std::string>& params_name, const std::vector<Platform::Var*>& params)>;
    
    static bool isPlatform(PlatformType type);
    static PlatformType platform();
    
    static void SetInitMehtod(PlatformInit method);
    static void SetPerformMehtod(PlatformPerform method);
    
    void Init(const std::string& file_name, const std::string& class_name, void* c_plus_plus_obj);
    void* Perform(const std::string& file_name, const std::string& method_name, bool is_set_delegate, const std::string& params_name, Platform::Var* params, ...) __attribute__((sentinel(0,1)));
    
    void* GetPlatformObj();
    
public:
    
    static PlatformInit init_fun_;
    static PlatformPerform perform_fun_;
    
private:
    
    void* platform_obj_;
    std::shared_ptr<void> c_plus_plus_obj_;
};

END_NAMESPACE_BASE_PLATFORM

#endif /* LB_BASE_PLATFORM_PCH_H */
