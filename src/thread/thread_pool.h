#ifndef LB_BASE_THREAD_POOL_PCH_H
#define LB_BASE_THREAD_POOL_PCH_H

#include "../pch.h"
#include "thread.h"

BEGIN_NAMESPACE_BASE_THREAD

class ThreadPool {
    
public:
    class Task {
    public:
        Task(Thread::ThreadFunc func, mach_port_t transfer_thread_id = 0);
        
        Thread::ThreadFunc func_;
        mach_port_t transfer_thread_id_ = 0;
    };
    
    ThreadPool(int32_t thread_num);
    
    static ThreadPool& DefaultPool();
    
    std::shared_ptr<Thread> GetMain();
    std::shared_ptr<Thread> GetIO();
    
    std::shared_ptr<Thread> Get(const std::string& name);
    std::shared_ptr<Thread> GetCurrent();
    
    void Schedule(Thread::Type type, Thread::ThreadFunc func);
    void ScheduleTransfer(Thread::Type type, Thread::ThreadFunc func);
    void ScheduleTransferBack(Thread::Type type, Thread::ThreadFunc func);
    void Terminate();
    
private:
    void Execute();
    void TaskSchedule(Thread::Type type, Task task);
    
private:
    std::map<mach_port_t, std::shared_ptr<Thread>> io_thread_map_;
    std::map<mach_port_t, std::shared_ptr<Thread>> any_thread_map_;
    std::map<mach_port_t, std::shared_ptr<Thread>> all_thread_map_;
    std::shared_ptr<Thread> main_thread_;
    
    int32_t thread_num_;
    std::queue<Task> task_queue_;
    
    std::condition_variable cv_;
    std::mutex cv_mutex;
    
    std::mutex mutex_;
    
    bool is_terminate_ = false;
    
    thread_local static mach_port_t transfer_thread_id_;
};

END_NAMESPACE_BASE_THREAD

#endif /* LB_BASE_THREAD_POOL_PCH_H */
