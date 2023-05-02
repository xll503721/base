#ifndef LB_BASE_STORAGE_MEMORY_PCH_H
#define LB_BASE_STORAGE_MEMORY_PCH_H


BEGIN_NAMESPACE_BASE_STORAGE_MEMORY

template <typename K, typename V>
class StorageMemory {
    
public:
    V& operator[](const K& key) {
        return map_[key];
    }
    
private:
    std::unordered_map<K, V> map_;
};

END_NAMESPACE_BASE_STORAGE_MEMORY

#endif /* LB_BASE_STORAGE_MEMORY_PCH_H */
