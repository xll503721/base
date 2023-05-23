#ifndef LB_BASE_MMAP_FILE_PCH_H
#define LB_BASE_MMAP_FILE_PCH_H

#include "../pch.h"
#include "mmap.h"

BEGIN_NAMESPACE_BASE_MMAP

class MmapFile {
    
public:
    virtual ~MmapFile();
    MmapFile() = default;
    
    bool Create(const std::string& path);
    bool Wirte(const std::string& data, size_t len);
    void SetFileName(const std::string& name);
    
private:
    
private:
    int fd_ = -1;
    Mmap mmap_;
};

END_NAMESPACE_BASE_MMAP

#endif /* LB_BASE_MMAP_FILE_PCH_H */
