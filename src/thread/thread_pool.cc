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

thread_local mach_port_t ThreadPool::transfer_thread_id_ = 0;

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
    all_thread_map_[main_thread_->GetId()] = main_thread_;
    pool_thread_ = std::make_shared<Thread>(std::bind(&ThreadPool::PoolExecute, this), Thread::Type::kPool);
    pool_thread_->Start();
    
    thread_num_ = thread_num;
    otlog_info << "create:" << thread_num_ << " count thread";
    for (int32_t i = 0; i < thread_num; i++) {
        std::shared_ptr<Thread> thread = std::make_shared<Thread>(std::bind(&ThreadPool::Execute, this), type);
        any_thread_map_[thread->GetId()] = thread;
        all_thread_map_[thread->GetId()] = thread;
        thread->Start();
    }
}

void ThreadPool::PoolExecute() {
    do {
        std::unique_lock<std::mutex> lock(pool_mutex_);
        pool_cv_.wait(lock, [this] {
            return pool_task_queue_.size() > 0;
        });
        
        auto task = pool_task_queue_.front();
        task();
        pool_task_queue_.pop();
    } while (true);
}

void ThreadPool::Execute() {
    do {
        otlog_info << "task queue size:" << task_queue_.size();
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] {
            return task_queue_.size() > 0;
        });
        
        auto task = task_queue_.front();
        otlog_info << "check task transfer_thread_id:" << static_cast<int32_t>(task.transfer_thread_id) << ", transfer_back_thread_id:" << static_cast<int32_t>(task.transfer_back_thread_id) << ", type:" << static_cast<int32_t>(task.type);
        
        auto TaskExecute = [=]() {
            otlog_info << "begin execute task func";
            task.func();
            task_queue_.pop();
            otlog_info << "end execute task func";
        };
        
        if (task.type == Thread::Type::kMain) {
            otlog_info << "add task to main thread, transfer thread id:" << task.transfer_thread_id;
            main_thread_->Post([=]() {
                transfer_thread_id_ = task.transfer_thread_id;
                task.func();
            });
            task_queue_.pop();
            continue;
        }
        
        auto thread_id = pthread_mach_thread_np(pthread_self());
        if (task.transfer_back_thread_id != 0) {
            if (task.transfer_back_thread_id == thread_id) {
                transfer_thread_id_ = 0;
                TaskExecute();
            }
            continue;
        }
        
        transfer_thread_id_ = task.transfer_thread_id;
        TaskExecute();
        
    } while (true && !pool_is_terminate_);
}

void ThreadPool::TaskSchedule(Thread::Type type, Task task) {
    otlog_info << "add task in type:" << std::to_string(static_cast<int32_t>(type)) << ", transfer thread id:" << task.transfer_thread_id;
    
    {
        std::unique_lock<std::mutex> lock(mutex_);
        task_queue_.push(task);
    }
    otlog_info << "notify all";
    cv_.notify_all();
}

void ThreadPool::ScheduleTransfer(Thread::Type type, Thread::ThreadFunc func) {
    otlog_info << "type:" << static_cast<int32_t>(type) << ", transfer thread id:" << transfer_thread_id_;
    
    Task task;
    task.func = func;
    task.transfer_thread_id = pthread_mach_thread_np(pthread_self());
    task.type = type;
    
    Thread::ThreadFunc pool_func = [=]() {
        TaskSchedule(type, task);
    };
    
    {
        std::unique_lock<std::mutex> lock(pool_mutex_);
        pool_task_queue_.push(pool_func);
    }
    pool_cv_.notify_one();
}

void ThreadPool::ScheduleTransferBack(Thread::Type type, Thread::ThreadFunc func) {
    otlog_info << "type:" << static_cast<int32_t>(type) << ", back to thread:" << transfer_thread_id_;
    if (transfer_thread_id_ == 0) {
        otlog_fault << "transfer thread id not set, please use `void ThreadPool::Schedule(Thread::Type type, Thread::ThreadFunc func)` instead";
        return;
    }
    
    Task task;
    task.func = func;
    task.transfer_back_thread_id = transfer_thread_id_;
    task.type = type;
    
    Thread::ThreadFunc pool_func = [=]() {
        TaskSchedule(type, task);
    };
    
    {
        std::unique_lock<std::mutex> lock(pool_mutex_);
        pool_task_queue_.push(pool_func);
    }
    pool_cv_.notify_one();
}

void ThreadPool::Schedule(Thread::Type type, Thread::ThreadFunc func) {
    Thread::ThreadFunc pool_func = [=]() {
        Task task;
        task.func = func;
        task.type = type;
        TaskSchedule(type, task);
    };
    
    {
        std::unique_lock<std::mutex> lock(pool_mutex_);
        pool_task_queue_.push(pool_func);
    }
    pool_cv_.notify_one();
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
