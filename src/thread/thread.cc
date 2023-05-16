//
//  thread.cc
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "thread.h"

BEGIN_NAMESPACE_BASE_THREAD

Thread::Thread(std::function<void(void)> func) {
    run_loop_ = std::make_shared<RunLoop>();
    std::thread inner_thread(func);
}

std::shared_ptr<RunLoop> Thread::GetCurrentRunLoop() {
    return run_loop_;
}

std::thread::id Thread::GetId() {
    
}

END_NAMESPACE_BASE_THREAD
