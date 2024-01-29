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

@property (nonatomic, strong) NSMutableArray<NSDictionary *> *dictParams;

@end

@implementation OTPlatform

- (instancetype)init
{
    self = [super init];
    if (self) {
        _dictParams = @[].mutableCopy;
        
        [self _setPlatformInfo];
    }
    return self;
}

- (void)_setPlatformInfo {
    BASE_PLATFORM::Platform::SetInitMehtod([=] (const std::string& class_name, std::shared_ptr<void> c_plus_plus_obj) {
        NSString *className = [NSString stringWithUTF8String:class_name.c_str()];
        id target = [self _platformInitWithClazzName:className];
        return (__bridge_retained void *)target;
    });
    
    BASE_PLATFORM::Platform::SetPerformDelegate([=](const void* platform_obj, const std::string& file_name, std::shared_ptr<void> c_plus_plus_obj) {
        if (!c_plus_plus_obj) {
            return;
        }
        
        id target = (__bridge NSObject *)platform_obj;
        if (!target) {
            return;
        }
        
        SEL getSelector = @selector(delegate);
        if (![target respondsToSelector:getSelector]) {
            return;
        }
        
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
        id<OTPlatformObjectDelegate> delegate = [target performSelector:getSelector];
#pragma clang diagnostic pop
        if (!delegate) {
            NSString *fileName = [[NSString stringWithUTF8String:file_name.c_str()] lowercaseString];
            NSArray<NSString *> *fileNames = [fileName componentsSeparatedByString:@"_"];
            __block NSString *ocFileName = (NSString *)kOTOnetenSDKPrefix;
            [fileNames enumerateObjectsUsingBlock:^(NSString * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                ocFileName = [ocFileName stringByAppendingString:[obj capitalizedString]];
            }];
            ocFileName = [ocFileName stringByAppendingString:kOTOnetenSDKDelegate];
            
            delegate = (id)[self _platformInitWithClazzName:ocFileName];
            SEL setSelector = @selector(setDelegate:);
            if (![target respondsToSelector:setSelector]) {
                return;
            }
            
            //only set, no alloc，new，copy or mutableCopy,
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
            [target performSelector:setSelector withObject:delegate];
#pragma clang diagnostic pop
        }
        
        SEL setCPlusPlusPrtSelector = @selector(setCPlusPlusPrt:);
        if (![delegate respondsToSelector:setCPlusPlusPrtSelector]) {
            return;
        }
        
        [delegate setCPlusPlusPrt:c_plus_plus_obj];
    });
    
    BASE_PLATFORM::Platform::SetPerformMehtod([=] (const void* platform_obj,
                                                   const std::string& file_name,
                                                   const std::string& method_name,
                                                   bool is_set_delegate,
                                                   const std::vector<std::string>& params_name,
                                                   const std::vector<BASE_PLATFORM::Platform::Var*>& params) -> std::shared_ptr<BASE_PLATFORM::Platform::Var> {
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
        
        [self _setWithParams:params inToInvocation:invocation];
        
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

- (instancetype)_platformInitWithClazzName:(NSString *)clazzName {
    Class clazz = NSClassFromString(clazzName);
    if (!clazz) {
        return nil;
    }
    
    id obj = [[clazz alloc] init];
    return obj;
}

- (void)_setWithParams:(const std::vector<BASE_PLATFORM::Platform::Var*>&)params inToInvocation:(NSInvocation *)invocation {
    __block int32_t argIndex = 2;
    [self.dictParams removeAllObjects];
    for (BASE_PLATFORM::Platform::Var* param: params) {
        auto type = param->GetType();
        switch (type) {
            case BASE_PLATFORM::Platform::Var::Type::kTypeString: {
                auto var = param->GetString();
                [invocation setArgument:&var atIndex:argIndex];
            }
                break;
            case BASE_PLATFORM::Platform::Var::Type::kTypeInt: {
                int32_t var = param->GetInt32();
                [invocation setArgument:&var atIndex:argIndex];
            }
                break;
            case BASE_PLATFORM::Platform::Var::Type::kTypeLong: {
                unsigned long var = param->GetUnsignedLong();
                [invocation setArgument:&var atIndex:argIndex];
            }
                break;
            case BASE_PLATFORM::Platform::Var::Type::kTypeMap: {
                std::map<std::string, BASE_PLATFORM::Platform::Var>* var_map = param->GetMap();
                NSMutableDictionary<NSString *, id> *dictVar = [NSMutableDictionary dictionary];
                [self.dictParams addObject:dictVar];
                for (auto iter = var_map->begin(); iter != var_map->end(); ++iter) {
                    NSString *key = [NSString stringWithUTF8String:iter->first.c_str()];
                    auto value = iter->second;

                    id ocValue = nil;
                    switch (value.GetType()) {
                        case BASE_PLATFORM::Platform::Var::Type::kTypeInt:
                            ocValue = [NSNumber numberWithInt:value.GetInt32()];
                            break;
                        case BASE_PLATFORM::Platform::Var::Type::kTypeString: {
                            ocValue = [NSString stringWithUTF8String:value.GetString().c_str()];
                        }

                        default:
                            break;
                    }
                    if (ocValue) {
                        dictVar[key] = ocValue;
                    }
                }
                
                [invocation setArgument:&dictVar atIndex:argIndex];
            }
                break;
                
            default:
                break;
        }
        argIndex++;
    }
}

@end
