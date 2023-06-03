//
//  thread.cc
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "run_loop.h"
#include <platform/platform.h>

BEGIN_NAMESPACE_BASE_THREAD

void RunLoop::SetupRunLoop() {
    
}

void RunLoop::SetupMainRunLoop() {
    run_loop_.SetupMianRunLoop();
}

bool RunLoop::IsMain() {
    return run_loop_.IsMain();
}

void RunLoop::Post(std::function<void(void)> action) {
    run_loop_.Post(action);
}

END_NAMESPACE_BASE_THREAD
