#ifndef LB_BASE_DEVICE_PCH_H
#define LB_BASE_DEVICE_PCH_H

#include "../pch.h"
#include <platform/platform.h>

BEGIN_NAMESPACE_BASE_DEVICE

class Device {
    
public:
    class File: public std::enable_shared_from_this<File> {
        
    public:
        File() = default;
        
        std::string GetDocumentsPath();
        std::string GetLibraryPath();
        std::string GetCachesPath();
        
    private:
        std::string documents_path_;
        std::string library_path_;
        std::string caches_path_;
        
        PLATFORM_GENERATE_NAME(OTFile)
    };
    
    Device();
    static Device& DefaultInstance();
    
    std::shared_ptr<File> GetFile();
    std::string GetUniqueDeviceId();
    std::string GetSystemVersion();
    std::string GetDeviceModel();
    
private:
    std::shared_ptr<File> file_;
    std::string device_id_;
    std::string system_version_;
};

END_NAMESPACE_BASE_DEVICE

#endif /* LB_BASE_DEVICE_PCH_H */
