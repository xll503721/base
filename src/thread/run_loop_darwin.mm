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

bool RunLoopDarwin::IsMain() {
    CFRunLoopRef current_ref_ = CFRunLoopGetCurrent();
    CFRunLoopRef main_ref_ = CFRunLoopGetMain();
    
    return (current_ref_ == main_ref_);
}

END_NAMESPACE_BASE_THREAD
