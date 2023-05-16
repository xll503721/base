#ifndef LB_BASE_RUN_LOOP_PCH_H
#define LB_BASE_RUN_LOOP_PCH_H

#include "../pch.h"
#include "run_loop_darwin.h"

BEGIN_NAMESPACE_BASE_THREAD

class RunLoopInterface {
    
public:
    virtual void SetupPlatformRunLoop() = 0;
    virtual bool IsMain() = 0;
    
};

class RunLoop: RunLoopInterface {
    
public:
    void SetupPlatformRunLoop() override;
    bool IsMain() override;
    
};

END_NAMESPACE_BASE_THREAD

#endif /* LB_BASE_RUN_LOOP_PCH_H */
