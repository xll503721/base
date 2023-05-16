#ifndef LB_BASE_THREAD_POOL_PCH_H
#define LB_BASE_THREAD_POOL_PCH_H

#include "../pch.h"
#include "thread.h"

BEGIN_NAMESPACE_BASE_THREAD

class ThreadPool {
    
public:
    ThreadPool(int32_t thread_num);
    
    static ThreadPool& DefaultPool();
    
    std::shared_ptr<Thread> GetMain();
    std::shared_ptr<Thread> GetIO();
    
    std::shared_ptr<Thread> Get(const std::string& name);
    std::shared_ptr<Thread> GetCurrent();
    
    void Schedule(std::function<void (void)> func);
    void Terminate();
    
private:
    void Execute();
    
private:
    std::map<std::thread::id, std::shared_ptr<Thread>> io_thread_map_;
    std::map<std::thread::id, std::shared_ptr<Thread>> any_thread_map_;
    std::shared_ptr<Thread> main_thread_;
    
    int32_t thread_num_;
    std::queue<std::function<void(void)>> task_queue_;
    
    std::condition_variable cv_;
    std::mutex cv_mutex;
    
    std::mutex mutex_;
    
    bool is_terminate_ = false;
};

END_NAMESPACE_BASE_THREAD

#endif /* LB_BASE_THREAD_POOL_PCH_H */
