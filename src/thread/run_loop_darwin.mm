//
//  thread.cc
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "run_loop_darwin.h"

BEGIN_NAMESPACE_BASE_THREAD

void RunLoopDarwin::SetupRunLoop() {
    ref_ = CFRunLoopGetCurrent();
    
    CFRunLoopPerformBlock(ref_, kCFRunLoopDefaultMode, ^{
        
    });
}

void RunLoopDarwin::SetupMianRunLoop() {
    ref_ = CFRunLoopGetMain();
}

bool RunLoopDarwin::IsMain() {
    CFRunLoopRef main_ref_ = CFRunLoopGetMain();
    
    return (ref_ == main_ref_);
}

void RunLoopDarwin::Post(std::function<void(void)> action) {
    if (!action) {
        return;
    }
    CFRunLoopPerformBlock(ref_, kCFRunLoopDefaultMode, ^{
        action();
    });
}

END_NAMESPACE_BASE_THREAD
