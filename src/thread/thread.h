#ifndef LB_BASE_THREAD_PCH_H
#define LB_BASE_THREAD_PCH_H

#include "../pch.h"
#include "run_loop.h"

BEGIN_NAMESPACE_BASE_THREAD

class Thread {
    
public:
    Thread() = default;
    explicit Thread(std::function<void(void)> func);
    std::thread::id GetId();
    
    std::shared_ptr<RunLoop> GetCurrentRunLoop();
    
private:
    std::shared_ptr<RunLoop> run_loop_;
};

END_NAMESPACE_BASE_THREAD

#endif /* LB_BASE_THREAD_PCH_H */
