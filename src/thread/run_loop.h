#ifndef LB_BASE_RUN_LOOP_PCH_H
#define LB_BASE_RUN_LOOP_PCH_H

#include "../pch.h"
#include "run_loop_darwin.h"
#include <log/log.h>

BEGIN_NAMESPACE_BASE_THREAD

class RunLoopInterface {
    
public:
    virtual void SetupRunLoop() = 0;
    virtual void SetupMainRunLoop() = 0;
    virtual bool IsMain() = 0;
    virtual void Post(std::function<void(void)> action) = 0;
};

class RunLoop: public RunLoopInterface {
    
public:
    void SetupRunLoop() override;
    void SetupMainRunLoop() override;
    bool IsMain() override;
    void Post(std::function<void(void)> action) override;
    
private:
#ifdef OS_IOS
    RunLoopDarwin run_loop_;
#endif
};

END_NAMESPACE_BASE_THREAD

#endif /* LB_BASE_RUN_LOOP_PCH_H */
