#ifndef LB_BASE_RUN_LOOP_DARWIN_PCH_H
#define LB_BASE_RUN_LOOP_DARWIN_PCH_H

#include "../pch.h"
#include "run_loop.h"

#ifdef __OBJC__

#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>

BEGIN_NAMESPACE_BASE_THREAD

class RunLoopDarwin {
    
public:
    void SetupRunLoop();
    
    bool IsMain();
    
private:
    CFRunLoopRef ref_;
};

END_NAMESPACE_BASE_THREAD

#endif /* LB_BASE_RUN_LOOP_DARWIN_PCH_H */
#endif
