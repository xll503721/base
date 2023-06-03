//
//  OTFile.m
//  SDKCommon
//
//  Created by li zhixuan on 2023/5/24.
//

#import "OTFile.h"

@implementation OTFile

- (NSString *)documentsPath {
    return [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
}

- (NSString *)libraryPath {
    return [NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) lastObject];
}

- (const char *)cachesPath {
    return [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject].UTF8String;
}

@end
