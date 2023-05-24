//
//  MmapFile.cc
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "mmap_file.h"
#include <stdio.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

BEGIN_NAMESPACE_BASE_MMAP

MmapFile::~MmapFile() {
    if (fd_ > 0) {
        close(fd_);
        fd_ = -1;
    }
    mmap_.Close();
}

bool MmapFile::Create(const std::string& path) {
    fd_ = open(path.c_str(), O_RDWR | O_CREAT | O_CLOEXEC, 0600);
    if (fd_ < 0) {
        printf("open fd failed\n");
        return false;
    }
    
    mmap_.Mapping(fd_);
    return true;
}

bool MmapFile::Wirte(const std::string& data, size_t len) {
    mmap_.Append(fd_, data, len);
}

void MmapFile::SetFileName(const std::string& name) {
    
}

END_NAMESPACE_BASE_MMAP
