//
//  OTPlatform.m
//  base_platform
//
//  Created by li zhixuan on 2023/6/19.
//

#import "OTPlatform.h"

#include "platform.h"

static NSString *kOTOnetenSDKPrefix = @"OT";
static NSString *kOTOnetenSDKDelegate = @"ObjectDelegate";

@interface OTPlatform ()

@end

@implementation OTPlatform

- (instancetype)init
{
    self = [super init];
    if (self) {
        [self _setPlatformInfo];
    }
    return self;
}

- (void)_setPlatformInfo {
    BASE_PLATFORM::Platform::SetInitMehtod([=] (const std::string& file_name, const std::string& class_name, void* c_plus_plus_obj) {
        NSString *className = [NSString stringWithUTF8String:class_name.c_str()];
        id target = [self platformInitWithClazzName:className];
        return (__bridge_retained void *)target;
    });
    
    BASE_PLATFORM::Platform::SetPerformMehtod([=] (const void* platform_obj, const std::string& file_name, const std::string& method_name, bool is_set_delegate, const std::vector<std::string>& params_name, const std::vector<BASE_PLATFORM::Platform::Var*>& params) -> std::shared_ptr<BASE_PLATFORM::Platform::Var> {
        if (!platform_obj) {
            return nullptr;
        }
        
        NSString *methodString = [NSString stringWithUTF8String:method_name.c_str()];
        
        id target = (__bridge id)platform_obj;
        SEL selector = NSSelectorFromString(methodString);
        if (![target respondsToSelector:selector]) {
            //alert
            return nullptr;
        }
        
        NSMethodSignature *methodSignature = [target methodSignatureForSelector:selector];
        NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:methodSignature];
        [invocation setTarget:target];
        [invocation setSelector:selector];
        
        __block int32_t argIndex = 2;
        NSMutableArray<id> *ocParmas = @[].mutableCopy;
        for (BASE_PLATFORM::Platform::Var* param: params) {
            auto type = param->GetType();
            switch (type) {
                case BASE_PLATFORM::Platform::Var::Type::kTypeInt: {
                    int32_t var = param->GetInt32();
                    [invocation setArgument:&var atIndex:argIndex];
                }
                    break;
                case BASE_PLATFORM::Platform::Var::Type::kTypeMap: {
                    std::map<std::string, BASE_PLATFORM::Platform::Var>* var_map = param->GetMap();
                    NSMutableDictionary<NSString *, id> *var_dict = [NSMutableDictionary dictionary];
                    for (auto iter = var_map->begin(); iter != var_map->end(); ++iter) {
                        NSString *key = [NSString stringWithUTF8String:iter->first.c_str()];
                        auto value = iter->second;

                        id ocValue = nil;
                        switch (value.GetType()) {
                            case BASE_PLATFORM::Platform::Var::Type::kTypeInt:
                                ocValue = [NSNumber numberWithInt:value.GetInt32()];
                                break;

                            default:
                                break;
                        }
                        if (ocValue) {
                            var_dict[key] = ocValue;
                        }
                    }
                    
                    [invocation setArgument:&var_dict atIndex:argIndex];
                }
                    break;
                    
                default:
                    break;
            }
            argIndex++;
        }
        
        [invocation invoke];
        
        std::shared_ptr<BASE_PLATFORM::Platform::Var> ret_var = nullptr;
        if (methodSignature.methodReturnLength) {
            __autoreleasing id returnValue = nil;
            [invocation getReturnValue:&returnValue];
            
            auto ret_var = std::make_shared<BASE_PLATFORM::Platform::Var>((__bridge void *)returnValue);
            return ret_var;
        }
        return ret_var;
    });
}

- (instancetype)platformInitWithClazzName:(NSString *)clazzName {
    Class clazz = NSClassFromString(clazzName);
    if (!clazz) {
        return nil;
    }
    
    id obj = [[clazz alloc] init];
    return obj;
}

- (id)platformPerformWithObject:(id)obj selectorString:(NSString *)selectorString params:(NSArray *)params {
    SEL selector = NSSelectorFromString(selectorString);
    if (![obj respondsToSelector:selector]) {
        //alert
        return;
    }
    
    NSMethodSignature *methodSignature = [obj methodSignatureForSelector:selector];
    NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:methodSignature];
    [invocation setTarget:obj];
    [invocation setSelector:selector];
    
    __block int32_t argIndex = 2;
    [params enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        [invocation setArgument:&obj atIndex:argIndex];
        argIndex++;
    }];
    
    [invocation invoke];
}

@end
