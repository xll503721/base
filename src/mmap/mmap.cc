//
//  mmap.cc
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "mmap.h"
#include <stdio.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

BEGIN_NAMESPACE_BASE_MMAP

Mmap::~Mmap() {
    Close();
}

bool Mmap::Mapping(int fd) {
    size_t page_size = getpagesize();
    
    //get file info, e.g. (st_size)
    struct stat st;
    if (fstat(fd, &st) != -1) {
        file_size_ = (size_t)st.st_size;
    }
    
    mmap_size_ = page_size;
    if (file_size_ == 0 || (file_size_ / page_size) != 0) {
        mmap_size_ = (file_size_ / page_size + 1) * page_size;
        if (ftruncate(fd, mmap_size_) != 0) {
            printf("ftruncate failed");
            return -1;
        }
    }
    
    data_ = (char *)mmap(nullptr, mmap_size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data_ == MAP_FAILED) {
        printf("mmap failed");
        return -1;
    }
    return 0;
}

bool Mmap::Append(const int fd, const std::string& data, size_t len) {
    if (!data_) {
        printf("append_mmap_data failed");
        return;
    }
    if (!CheckSize(len)) {
        ExpandCapacity(fd);
    }
    memcpy(static_cast<uint8_t*>(data_) + offset_, data.c_str(), len);
    offset_ += len;
}

void Mmap::Close() {
    if (data_) {
        if (munmap(data_, file_size_) != 0) {
            printf("close_res munmap failed");
        }
        data_ = nullptr;
        file_size_ = 0;
    }
    mmap_size_ = 0;
    offset_ = 0;
}

bool Mmap::CheckSize(size_t len) {
    if (offset_ + len > mmap_size_) {
        return false;
    }
    return true;
}

void Mmap::ExpandCapacity(const int fd) {
    off_t expand_capacity_size = mmap_size_ + getpagesize();
    if (ftruncate(fd, expand_capacity_size) != 0) {
        printf("expand_capacity ftruncate failed");
        return;
    }
    if (munmap(data_, mmap_size_) != 0) {
        printf("expand_capacity munmap failed");
        return;
    }
    mmap_size_ = expand_capacity_size;
    data_ = (char *)mmap(nullptr, mmap_size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data_ == MAP_FAILED) {
        printf("expand_capacity mmap failed");
        return;
    }
}

END_NAMESPACE_BASE_MMAP
