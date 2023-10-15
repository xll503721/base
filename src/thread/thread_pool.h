#ifndef LB_BASE_THREAD_POOL_PCH_H
#define LB_BASE_THREAD_POOL_PCH_H

#include "../pch.h"
#include "thread.h"

BEGIN_NAMESPACE_BASE_THREAD

class ThreadPool {
    
public:
    struct Task {
        Thread::ThreadFunc func;
        Thread::Type type = Thread::Type::kOther;
    };
    struct PoolTask {
        Thread::ThreadFunc func;
    };
    
    ThreadPool(int32_t thread_num, BASE_THREAD::Thread::Type type = BASE_THREAD::Thread::Type::kOther);
    ~ThreadPool();
    
    static ThreadPool& DefaultPool();
    
    std::shared_ptr<Thread> GetMain();
    std::shared_ptr<Thread> GetIO();
    
    std::shared_ptr<Thread> Get(const std::string& name);
    std::shared_ptr<Thread> GetCurrent();
    
    void Schedule(Thread::Type type, Thread::ThreadFunc func);
    void Terminate();
    
private:
    void Init(int32_t thread_num, BASE_THREAD::Thread::Type type);
    void Execute();
    void TaskSchedule(Task task);
    
private:
    std::map<mach_port_t, std::shared_ptr<Thread>> io_thread_map_;
    std::map<mach_port_t, std::shared_ptr<Thread>> any_thread_map_;
    std::map<mach_port_t, std::shared_ptr<Thread>> all_thread_map_;
    std::vector<std::shared_ptr<Thread>> all_thread_vector_;
    std::shared_ptr<Thread> main_thread_;
    std::shared_ptr<Thread> https_thread_;
    
    int32_t thread_num_;
    std::queue<Task> task_queue_;
    
    std::condition_variable cv_;
    std::mutex mutex_;
    
    bool pool_is_terminate_ = false;
};

END_NAMESPACE_BASE_THREAD

#endif /* LB_BASE_THREAD_POOL_PCH_H */
