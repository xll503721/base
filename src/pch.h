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

#define BASE_STRING base_string
#define BEGIN_NAMESPACE_BASE_STRING BEGIN_NAMESPACE(BASE_STRING)
#define END_NAMESPACE_BASE_STRING END_NAMESPACE()

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE() }

#endif /* LB_BASE_PCH_H */
