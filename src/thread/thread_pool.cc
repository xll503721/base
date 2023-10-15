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
    static ThreadPool pool(5);
    return pool;
}

ThreadPool::~ThreadPool() {
    otlog_info << "~ThreadPool";
}

ThreadPool::ThreadPool(int32_t thread_num, BASE_THREAD::Thread::Type type){
    Init(thread_num, type);
}

void ThreadPool::Init(int32_t thread_num, BASE_THREAD::Thread::Type type) {
    main_thread_ = std::make_shared<Thread>(Thread::Type::kMain);
    
    thread_num_ = thread_num;
    otlog_info << "create:" << thread_num_ << " count thread";
    for (int32_t i = 0; i < thread_num; i++) {
        std::shared_ptr<Thread> thread = std::make_shared<Thread>(std::bind(&ThreadPool::Execute, this), type);
        all_thread_vector_.push_back(thread);
        thread->Start();
    }
}

void ThreadPool::Execute() {
    do {
        otlog_info << "task queue size:" << task_queue_.size();
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] {
            return task_queue_.size() > 0;
        });
        
        auto task = task_queue_.front();
        otlog_info << "type:" << static_cast<int32_t>(task.type);
        
        auto TaskExecute = [=]() {
            otlog_info << "begin execute task func";
            task.func();
            task_queue_.pop();
            otlog_info << "end execute task func";
        };
        
        if (task.type == Thread::Type::kMain) {
            otlog_info << "post task func to main";
            main_thread_->Post([=]() {
                otlog_info << "begin execute task func";
                task.func();
                otlog_info << "end execute task func";
            });
            task_queue_.pop();
            otlog_info << "end post task func to main";
            continue;
        }
        
        TaskExecute();
        
    } while (true && !pool_is_terminate_);
}

void ThreadPool::TaskSchedule(Task task) {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        task_queue_.push(task);
    }
    otlog_info << "notify all";
    cv_.notify_all();
}

void ThreadPool::Schedule(Thread::Type type, Thread::ThreadFunc func) {
    Task task;
    task.func = func;
    task.type = type;
    
    TaskSchedule(task);
}

void ThreadPool::Terminate() {
    pool_is_terminate_ = true;
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
    mach_port_t thread_id = pthread_mach_thread_np(pthread_self());
    
    std::shared_ptr<Thread> thread = std::make_shared<Thread>();
    if (all_thread_map_.find(thread_id) != all_thread_map_.end()) {
        thread = all_thread_map_[thread_id];
        return thread;
    }
    return thread;
}

END_NAMESPACE_BASE_THREAD
