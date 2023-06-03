#include "device.h"

BEGIN_NAMESPACE_BASE_DEVICE

Device& Device::DefaultInstance() {
    static Device device;
    return device;
}

Device::Device() {
    file_ = std::make_shared<File>();
}

std::shared_ptr<Device::File> Device::GetFile() {
    return file_;
}

std::string Device::GetUniqueDeviceId() {
    if (device_id_.empty()) {
        
    }
    return device_id_;
}

std::string Device::GetSystemVersion() {
    if (system_version_.empty()) {
        
    }
    return system_version_;
}

#pragma mark - File

std::string Device::File::GetDocumentsPath() {
    if (documents_path_.empty()) {
    }
    return documents_path_;
}

std::string Device::File::GetLibraryPath() {
    if (library_path_.empty()) {
//        PLATFORM_PERFORM_TEST()
    }
    return library_path_;
}

std::string Device::File::GetCachesPath() {
    if (caches_path_.empty()) {
        PLATFORM_INVOKE()
        caches_path_ = static_cast<char*>(GET_PLATFORM_INVOKE_RESULT);
    }
    return caches_path_;
}

END_NAMESPACE_BASE_DEVICE
