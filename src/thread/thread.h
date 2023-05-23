#ifndef LB_BASE_THREAD_PCH_H
#define LB_BASE_THREAD_PCH_H

#include "../pch.h"
#include "run_loop.h"

BEGIN_NAMESPACE_BASE_THREAD

class Thread {
    
public:
    using ThreadFunc = std::function<void()>;
    
    Thread() = default;
    Thread(ThreadFunc func);
    std::thread::id GetId();
    
    std::shared_ptr<RunLoop> GetCurrentRunLoop();
    
    void Start();
    
private:
    std::shared_ptr<RunLoop> run_loop_;
    ThreadFunc func_;
};

END_NAMESPACE_BASE_THREAD

#endif /* LB_BASE_THREAD_PCH_H */
