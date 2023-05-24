#ifndef LB_BASE_DEVICE_PCH_H
#define LB_BASE_DEVICE_PCH_H

#include "../pch.h"

BEGIN_NAMESPACE_BASE_DEVICE

#define OT_DEVICE Device::DefaultInstance()
#define OT_FILE Device::DefaultInstance().GetFile()

class Device {
    
public:
    class File {
        
    public:
        File() = default;
        
        std::string Documents();
        std::string Library();
        std::string Caches();
    };
    
    static Device& DefaultInstance();
    
    File& GetFile();
    
private:
    File file_;
};

END_NAMESPACE_BASE_DEVICE

#endif /* LB_BASE_DEVICE_PCH_H */
