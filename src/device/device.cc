#include "device.h"

BEGIN_NAMESPACE_BASE_DEVICE

Device& Device::DefaultInstance() {
    static Device device;
    return device;
}

Device::File& Device::GetFile() {
    return file_;
}

std::string Device::File::Documents() {
    return "";
}

std::string Device::File::Library() {
    return "";
}

std::string Device::File::Caches() {
    return "";
}

END_NAMESPACE_BASE_DEVICE
