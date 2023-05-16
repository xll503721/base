#ifndef LB_BASE_PCH_H
#define LB_BASE_PCH_H

#include <stdio.h>
#include <string.h>
#include <map>
#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <sstream>
#include <unordered_map>
#include <thread>
#include <condition_variable>

#define BASE_THREAD base_thread
#define BEGIN_NAMESPACE_BASE_THREAD BEGIN_NAMESPACE(BASE_THREAD)
#define END_NAMESPACE_BASE_THREAD END_NAMESPACE()

#define BASE_JSON base_json
#define BEGIN_NAMESPACE_BASE_JSON BEGIN_NAMESPACE(BASE_JSON)
#define END_NAMESPACE_BASE_JSON END_NAMESPACE()

#define BASE_STORAGE_MEMORY base_storage_memory
#define BEGIN_NAMESPACE_BASE_STORAGE_MEMORY BEGIN_NAMESPACE(BASE_STORAGE_MEMORY)
#define END_NAMESPACE_BASE_STORAGE_MEMORY END_NAMESPACE()

#define BASE_STRING base_string
#define BEGIN_NAMESPACE_BASE_STRING BEGIN_NAMESPACE(BASE_STRING)
#define END_NAMESPACE_BASE_STRING END_NAMESPACE()

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE() }

#endif /* LB_BASE_PCH_H */
