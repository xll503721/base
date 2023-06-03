#ifndef LB_BASE_RUN_LOOP_DARWIN_PCH_H
#define LB_BASE_RUN_LOOP_DARWIN_PCH_H

#include "../pch.h"

#include <CoreFoundation/CoreFoundation.h>

BEGIN_NAMESPACE_BASE_THREAD

class RunLoopDarwin {
    
public:
    
    void SetupRunLoop();
    void SetupMianRunLoop();
    
    bool IsMain();
    void Post(std::function<void(void)> action);
    
private:
    CFRunLoopRef ref_;
};

END_NAMESPACE_BASE_THREAD

#endif /* LB_BASE_RUN_LOOP_DARWIN_PCH_H */
