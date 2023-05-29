//
//  OTDevice.m
//  base_device
//
//  Created by li zhixuan on 2023/5/24.
//

#import "OTDevice.h"

@implementation OTDevice

- (NSString *)GetUniqueDeviceId {
    return [UIDevice currentDevice].identifierForVendor.UUIDString;
}

@end
