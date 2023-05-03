#ifndef LB_BASE_STORAGE_MEMORY_PCH_H
#define LB_BASE_STORAGE_MEMORY_PCH_H

#include "../../pch.h"

BEGIN_NAMESPACE_BASE_STORAGE_MEMORY

template <typename K, typename V>
class StorageMemory {
    
public:
    V& operator[](const K& key) {
        return map_[key];
    }
    
    bool Remove(const K& key)  {
        if (map_.find(key) != map_.end()) {
            map_.erase(key);
            return true;
        }
        return false;
    }
    
private:
    std::map<K, V> map_;
};

END_NAMESPACE_BASE_STORAGE_MEMORY

#endif /* LB_BASE_STORAGE_MEMORY_PCH_H */
