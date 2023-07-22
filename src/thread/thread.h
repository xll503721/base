#ifndef LB_BASE_THREAD_PCH_H
#define LB_BASE_THREAD_PCH_H

#include "../pch.h"
#include "run_loop.h"

BEGIN_NAMESPACE_BASE_THREAD

class Thread {
    
public:
    enum class Type {
        kMain = 1,
        kIO,
        kHTTP,
        kOther,
        kPool,
    };
    
    using ThreadFunc = std::function<void()>;
    
    Thread() = default;
    Thread(Type type);
    Thread(ThreadFunc func, Type type = Type::kOther);
    
    static bool IsMain();
    
    mach_port_t GetId();
    
    std::shared_ptr<RunLoop> GetCurrentRunLoop();
    
    void Start();
    void Post(ThreadFunc func);
    
private:
    std::shared_ptr<RunLoop> run_loop_;
    ThreadFunc func_;
    Type type_;
};

END_NAMESPACE_BASE_THREAD

#endif /* LB_BASE_THREAD_PCH_H */
