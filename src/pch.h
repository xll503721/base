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
#include <sstream>
#include <pthread.h>
#include <fstream>

#define BASE_DEVICE BASE(device)
#define BEGIN_NAMESPACE_BASE_DEVICE BEGIN_NAMESPACE(BASE_DEVICE)
#define END_NAMESPACE_BASE_DEVICE END_NAMESPACE()

#define BASE_LOG BASE(log)
#define BEGIN_NAMESPACE_BASE_LOG BEGIN_NAMESPACE(BASE_LOG)
#define END_NAMESPACE_BASE_LOG END_NAMESPACE()

#define BASE_MMAP BASE(mmap)
#define BEGIN_NAMESPACE_BASE_MMAP BEGIN_NAMESPACE(BASE_MMAP)
#define END_NAMESPACE_BASE_MMAP END_NAMESPACE()

#define BASE_THREAD BASE(thread)
#define BEGIN_NAMESPACE_BASE_THREAD BEGIN_NAMESPACE(BASE_THREAD)
#define END_NAMESPACE_BASE_THREAD END_NAMESPACE()

#define BASE_JSON BASE(json)
#define BEGIN_NAMESPACE_BASE_JSON BEGIN_NAMESPACE(BASE_JSON)
#define END_NAMESPACE_BASE_JSON END_NAMESPACE()

#define BASE_STORAGE_MEMORY BASE(memory)
#define BEGIN_NAMESPACE_BASE_STORAGE_MEMORY BEGIN_NAMESPACE(BASE_STORAGE_MEMORY)
#define END_NAMESPACE_BASE_STORAGE_MEMORY END_NAMESPACE()

#define BASE_STRING BASE(string)
#define BEGIN_NAMESPACE_BASE_STRING BEGIN_NAMESPACE(BASE_STRING)
#define END_NAMESPACE_BASE_STRING END_NAMESPACE()

#define BASE(NAME) ot_base_##NAME

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE() }

#endif /* LB_BASE_PCH_H */
