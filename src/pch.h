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

#define LBBASE base
#define BEGIN_NAMESPACE_LBBASE BEGIN_NAMESPACE(LBBASE)
#define END_NAMESPACE_LBBASE END_NAMESPACE()

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE() }

#endif /* LB_BASE_PCH_H */
