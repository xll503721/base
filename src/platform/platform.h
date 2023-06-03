//
//  platform.h
//  OneTenSDK
//
//  Created by li zhixuan on 2023/4/9.
//

#ifndef LB_BASE_PLATFORM_PCH_H
#define LB_BASE_PLATFORM_PCH_H

#include "../pch.h"
#include <log/log.h>

BEGIN_NAMESPACE_BASE_PLATFORM

#define PLATFORM_GENERATE()\
std::string platform_obj_name_;\
std::shared_ptr<BASE_PLATFORM::Platform> platform_;\
public:\
std::shared_ptr<BASE_PLATFORM::Platform> GetPlatform() {\
    if (!platform_) {\
        platform_ = std::make_shared<BASE_PLATFORM::Platform>(platform_obj_name_, shared_from_this());\
        std::string delegate_file(__FILE_NAME__);\
        platform_->Init(delegate_file, platform_obj_name_, shared_from_this().get());\
    }\
    return platform_;\
}\
void* GetPlatformObj() {\
    return platform_->GetPlatformObj();\
}\

#define SET_PLATFORM_GENERATE_NAME(PLATFORM_OBJ_NAME)\
platform_obj_name_ = PLATFORM_OBJ_NAME;\

#define PLATFORM_GENERATE_NAME(PLATFORM_OBJ_NAME)\
std::string platform_obj_name_ = #PLATFORM_OBJ_NAME;\
std::shared_ptr<BASE_PLATFORM::Platform> platform_;\
public:\
std::shared_ptr<BASE_PLATFORM::Platform> GetPlatform() {\
    if (!platform_) {\
        platform_ = std::make_shared<BASE_PLATFORM::Platform>(platform_obj_name_, shared_from_this());\
        std::string delegate_file(__FILE_NAME__);\
        platform_->Init(delegate_file, platform_obj_name_, shared_from_this().get());\
    }\
    return platform_;\
}\

#define PLATFORM_VAR_GENERATE(VALUE)\
BASE_PLATFORM::Platform::GetVar(VALUE);

#define PLATFORM_INVOKE(...) \
std::string parmas_str = #__VA_ARGS__;\
std::string file(__FILE_NAME__);\
std::string func(__PRETTY_FUNCTION__);\
auto PLATFORM_INVOKE_RESULT = GetPlatform()->Perform(file, func, false, parmas_str, ##__VA_ARGS__, nullptr);\

#define GET_PLATFORM_INVOKE_RESULT PLATFORM_INVOKE_RESULT->GetPtr()

class Platform {
    
public:
    
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
            kTypeVector,
            kTypeVoid
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
        
        Var(const std::string& value) {
            value_.c_ = const_cast<char*>(value.c_str());
            type_ = Type::kTypeDouble;
        }
        
        Var(const char* value) {
            value_.c_ = const_cast<char*>(value);
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
        
        Var(void* value) {
            value_.ptr_ = value;
            type_ = Type::kTypeVoid;
        }
        
        Type GetType() {
            return type_;
        }
        
        int32_t GetInt32() {
            if (GetType() != Type::kTypeInt) {
                otlog_fault << "type error";
            }
            return value_.int32_;
        }
        
        int32_t GetBool() {
            if (GetType() != Type::kTypeBool) {
                otlog_fault << "type error";
            }
            return value_.boolean_;
        }
        
        std::unordered_map<std::string, Var>* GetMap() {
            if (GetType() != Type::kTypeMap) {
                otlog_fault << "type error";
            }
            return value_.map_;
        }
        
        std::string GetString() {
            if (GetType() != Type::kTypeString) {
                otlog_fault << "type error";
            }
            return value_.c_;
        }
        
        void* GetPtr() {
            if (GetType() != Type::kTypeVoid) {
                otlog_fault << "type error";
            }
            return value_.ptr_;
        }
        
    private:
        union ValueHolder {
            int64_t int64_;
            int32_t int32_;
            bool boolean_;
            double double_;
            float float_;
            char* c_;
            void* ptr_;
            std::unordered_map<std::string, Var>* map_;
            std::vector<Var>* vector_;
        } value_;
        Type type_;
    };
    
    enum class PlatformType {
        kPlatformTypeiOS,
        kPlatformTypeMac,
        kPlatformTypeAndroid,
    };
    
    Platform() = default;
    Platform(const std::string& class_name, std::shared_ptr<void> c_plus_plus_obj);
    
    static Var GetVar(Var var);
    
    using PlatformInit = std::function<void* (const std::string& file_name, const std::string& class_name, void* c_plus_plus_obj)>;
    using PlatformPerform = std::function<std::shared_ptr<Var> (const void* platform_obj, const std::string& file_name, const std::string& method_name, bool is_set_delegate, const std::vector<std::string>& params_name, const std::vector<Platform::Var*>& params)>;
    
    void Init(const std::string& file_name, const std::string& class_name, void* c_plus_plus_obj);
    std::shared_ptr<Var> Perform(const std::string& file_name, const std::string& method_name_full, bool is_set_delegate, const std::string& params_name, Platform::Var* params, ...) __attribute__((sentinel(0,1)));
    
    static bool isPlatform(PlatformType type);
    static PlatformType platform();
    
    static void SetInitMehtod(PlatformInit method);
    static void SetPerformMehtod(PlatformPerform method);
    
    void* GetPlatformObj();
    
private:
    bool ParseMethod(const std::string& method_name_full, std::string& retrun_type, std::string& only_method);
    void ParseMethodStyleiOS(const std::vector<std::string>& params_name_vector, std::string& only_method);
    
public:
    
    static PlatformInit init_fun_;
    static PlatformPerform perform_fun_;
    
private:
    
    void* platform_obj_;
    std::shared_ptr<void> c_plus_plus_obj_;
};

END_NAMESPACE_BASE_PLATFORM

#endif /* LB_BASE_PLATFORM_PCH_H */
