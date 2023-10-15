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
        run_loop_->Post([=]() {
            id_ = pthread_mach_thread_np(pthread_self());
        });
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

std::weak_ptr<Thread> Thread::GetCurrentThread() {
    return self;
}

mach_port_t Thread::GetId() {
    return id_;
}

void Thread::Start() {
    if (type_ == Type::kMain) {
        otlog_fault << "this thread is main, could not start";
        return;
    }

    std::thread thread([=]() {
        id_ = pthread_mach_thread_np(pthread_self());
        self = shared_from_this();
        func_();
    });
    thread.detach();
}

void Thread::Post(ThreadFunc func) {
    run_loop_->Post(func);
}

bool Thread::IsMain() {
    return true;
}

END_NAMESPACE_BASE_THREAD
