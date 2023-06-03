//
//  thread.cc
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "thread_pool.h"
#include "thread.h"
#include <log/log.h>

BEGIN_NAMESPACE_BASE_THREAD

ThreadPool &ThreadPool::DefaultPool() {
    static ThreadPool pool(2);
    return pool;
}

ThreadPool::ThreadPool(int32_t thread_num) {
    main_thread_ = std::make_shared<Thread>(Thread::Type::kMain);
    std::lock_guard<std::mutex> lock(mutex_);
    
    thread_num_ = thread_num;
    otlog_info << "create:" << thread_num_ << " count thread";
    for (int32_t i = 0; i < thread_num; i++) {
        std::shared_ptr<Thread> thread = std::make_shared<Thread>(std::bind(&ThreadPool::Execute, this), BASE_THREAD::Thread::Type::kOther);
        any_thread_map_[thread->GetId()] = thread;
        thread->Start();
    }
}

void ThreadPool::Execute() {
    do {
        otlog_info << "task queue size:" << std::to_string(task_queue_.size());
        std::unique_lock<std::mutex> lock(cv_mutex);
        cv_.wait(lock, [this] {
            return task_queue_.size() > 0;
        });
        
        otlog_info << "execute task";
        auto func = task_queue_.front();
        func();
        task_queue_.pop();
    } while (true && !is_terminate_);
}

void ThreadPool::Schedule(Thread::Type type, Thread::ThreadFunc func) {
    if (type == Thread::Type::kMain) {
        otlog_info << "add task to main thread";
        main_thread_->Post(func);
        return;
    }
    otlog_info << "add task in type:" << std::to_string(static_cast<int32_t>(type));
    task_queue_.push(func);
    std::unique_lock<std::mutex> lock(cv_mutex);
    cv_.notify_all();
}

void ThreadPool::Terminate() {
    is_terminate_ = true;
}

std::shared_ptr<Thread> ThreadPool::GetMain() {
    bool is_main_thread = main_thread_->GetCurrentRunLoop()->IsMain();
    if (is_main_thread) {
        return main_thread_;
    }
    return nullptr;
}

std::shared_ptr<Thread> ThreadPool::GetIO() {
    
}

std::shared_ptr<Thread> ThreadPool::Get(const std::string& name) {
    
}

std::shared_ptr<Thread> ThreadPool::GetCurrent() {
    std::thread::id this_id = std::this_thread::get_id();
    
}

END_NAMESPACE_BASE_THREAD
