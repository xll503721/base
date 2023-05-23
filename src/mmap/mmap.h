#ifndef LB_BASE_MMAP_PCH_H
#define LB_BASE_MMAP_PCH_H

#include "../pch.h"

BEGIN_NAMESPACE_BASE_MMAP

class Mmap {
    
public:
    virtual ~Mmap();
    Mmap() = default;
    
    bool Mapping(int fd);
    bool Append(const int fd, const std::string& data, size_t len);
    
    void Close();
private:
    bool CheckSize(size_t len);
    void ExpandCapacity(const int fd);
    
private:
    void *data_ = nullptr;

    size_t file_size_ = 0;
    size_t mmap_size_ = 0;
    size_t offset_ = 0;
};

END_NAMESPACE_BASE_MMAP

#endif /* LB_BASE_MMAP_FILE_PCH_H */
