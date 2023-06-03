//
//  thread.cc
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "thread.h"
#include <log/log.h>

BEGIN_NAMESPACE_BASE_THREAD

Thread::Thread(Type type):type_(type) {
    run_loop_ = std::make_shared<RunLoop>();
    if (type == Type::kMain) {
        run_loop_->SetupMainRunLoop();
    }
}

Thread::Thread(ThreadFunc func, Type type):
func_(func),
type_(type) {
    run_loop_ = std::make_shared<RunLoop>();
}

std::shared_ptr<RunLoop> Thread::GetCurrentRunLoop() {
    return run_loop_;
}

std::thread::id Thread::GetId() {
    
}

void Thread::Start() {
    if (type_ == Type::kMain) {
        otlog_fault << "this thread is main, could not start";
        return;
    }
    std::thread thread(func_);
    thread.detach();
}

void Thread::Post(ThreadFunc func) {
    run_loop_->Post(func);
}

END_NAMESPACE_BASE_THREAD
