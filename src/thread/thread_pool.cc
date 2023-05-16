//
//  thread.cc
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "thread_pool.h"
#include "thread.h"

BEGIN_NAMESPACE_BASE_THREAD

ThreadPool &ThreadPool::DefaultPool() {
    static ThreadPool pool(2);
    return pool;
}

ThreadPool::ThreadPool(int32_t thread_num) {
    main_thread_ = std::make_shared<Thread>();
    std::lock_guard<std::mutex> lock(mutex_);
    
    thread_num_ = thread_num;
    for (int32_t i = 0; i < thread_num; i++) {
        std::shared_ptr<Thread> thread = std::make_shared<Thread>();
        any_thread_map_[thread->GetId()] = thread;
    }
}

void ThreadPool::Execute() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    do {
        std::unique_lock<std::mutex> lock(cv_mutex);
        cv_.wait(lock, [this] {
            return task_queue_.size() > 0;
        });
        
        auto func = task_queue_.front();
        func();
        task_queue_.pop();
    } while (true && !is_terminate_);
    
    
}

void ThreadPool::Schedule(std::function<void (void)> func) {
    std::lock_guard<std::mutex> lock(mutex_);
    {
        task_queue_.push(func);
        std::unique_lock<std::mutex> lock(cv_mutex);
        cv_.notify_all();
    }
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
