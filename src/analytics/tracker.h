#ifndef LB_BASE_TRACKER_PCH_H
#define LB_BASE_TRACKER_PCH_H

#include "../pch.h"

BEGIN_NAMESPACE_BASE_ANALYTICS

class Tracker {

public:
    Tracker() = default;
    
    void Send(const std::string& event_name, std::map<std::string, std::string>& event_properties);
};

END_NAMESPACE_BASE_ANALYTICS

#endif /* LB_BASE_TRACKER_PCH_H */
