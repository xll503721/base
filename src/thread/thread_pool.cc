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

ThreadPool::ThreadPool(int32_t thread_num) {
    main_thread_ = std::make_shared<Thread>(Thread::Type::kMain);
    all_thread_map_[main_thread_->GetId()] = main_thread_;
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    thread_num_ = thread_num;
    otlog_info << "create:" << thread_num_ << " count thread";
    for (int32_t i = 0; i < thread_num; i++) {
        std::shared_ptr<Thread> thread = std::make_shared<Thread>(std::bind(&ThreadPool::Execute, this), BASE_THREAD::Thread::Type::kOther);
        any_thread_map_[thread->GetId()] = thread;
        all_thread_map_[thread->GetId()] = thread;
        thread->Start();
    }
}

void ThreadPool::Execute() {
    do {
        otlog_info << "task queue size:" << task_queue_.size();
        std::unique_lock<std::mutex> lock(cv_mutex);
        cv_.wait(lock, [this] {
            return task_queue_.size() > 0;
        });
        
        otlog_info << "check task transfer_thread_id:" << static_cast<int32_t>(transfer_thread_id_);
        auto task = task_queue_.front();
        
        auto TaskExecute = [=]() {
            otlog_info << "execute task func";
            task.func_();
            task_queue_.pop();
        };
        
        if (task.transfer_thread_id_ == 0) {
            TaskExecute();
            continue;
        }
        
        auto thread_id = pthread_mach_thread_np(pthread_self());
        if (task.transfer_thread_id_ == thread_id) {
            TaskExecute();
        }
        
    } while (true && !is_terminate_);
}

void ThreadPool::TaskSchedule(Thread::Type type, Task task) {
    if (type == Thread::Type::kMain) {
        otlog_info << "add task to main thread, transfer thread id:" << task.transfer_thread_id_;
        main_thread_->Post([=]() {
            transfer_thread_id_ = task.transfer_thread_id_;
            task.func_();
        });
        return;
    }
    
    otlog_info << "add task in type:" << std::to_string(static_cast<int32_t>(type)) << ", transfer thread id:" << task.transfer_thread_id_;
    
    task_queue_.push(task);
    std::unique_lock<std::mutex> lock(cv_mutex);
    cv_.notify_all();
}

void ThreadPool::ScheduleTransfer(Thread::Type type, Thread::ThreadFunc func) {
    otlog_info << "type:" << static_cast<int32_t>(type) << ", transfer thread id:" << transfer_thread_id_;
    Task task(func, pthread_mach_thread_np(pthread_self()));
    TaskSchedule(type, task);
}

void ThreadPool::ScheduleTransferBack(Thread::Type type, Thread::ThreadFunc func) {
    otlog_info << "type:" << static_cast<int32_t>(type) << ", back to thread:" << transfer_thread_id_;
    if (transfer_thread_id_ == 0) {
        otlog_fault << "transfer thread id not set, please use `void ThreadPool::Schedule(Thread::Type type, Thread::ThreadFunc func)` instead";
        Schedule(type, func);
        return;
    }
    
    Task task(func, transfer_thread_id_);
    TaskSchedule(type, task);
}

void ThreadPool::Schedule(Thread::Type type, Thread::ThreadFunc func) {
    Task task(func);
    TaskSchedule(type, task);
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
    mach_port_t thread_id = pthread_mach_thread_np(pthread_self());
    
    std::shared_ptr<Thread> thread = std::make_shared<Thread>();
    if (all_thread_map_.find(thread_id) != all_thread_map_.end()) {
        thread = all_thread_map_[thread_id];
        return thread;
    }
    return thread;
}

ThreadPool::Task::Task(Thread::ThreadFunc func, mach_port_t transfer_thread_id):
func_(func),
transfer_thread_id_(transfer_thread_id) {
}

END_NAMESPACE_BASE_THREAD
