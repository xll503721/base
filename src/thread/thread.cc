//
//  thread.cc
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "thread.h"

BEGIN_NAMESPACE_BASE_THREAD

Thread::Thread(ThreadFunc func): func_(func) {
    run_loop_ = std::make_shared<RunLoop>();
}

std::shared_ptr<RunLoop> Thread::GetCurrentRunLoop() {
    return run_loop_;
}

std::thread::id Thread::GetId() {
    
}

void Thread::Start() {
    std::thread thread(func_);
}

END_NAMESPACE_BASE_THREAD
