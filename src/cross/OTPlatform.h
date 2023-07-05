//
//  OTPlatform.h
//  base_platform
//
//  Created by li zhixuan on 2023/6/19.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol OTPlatformObjectDelegate <NSObject>

@required
@property void* cPlusPlusPrt;

@end

@interface OTPlatform : NSObject

@end

NS_ASSUME_NONNULL_END
