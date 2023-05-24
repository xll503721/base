//
//  oneten_ad_sdk.cpp
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "string.h"

BEGIN_NAMESPACE_BASE_STRING

// trim from start (in place)
void LeftTrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) {
      return ch != ' ';
    }));
}

// trim from end (in place)
void RightTrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) {
      return ch != ' ';
    }).base(), s.end());
}

// trim from both ends (in place)
void Trim(std::string& s) {
    LeftTrim(s);
    RightTrim(s);
}


void ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

bool StartsWith(const std::string& target_str, const std::string& prefix) {
    if (prefix.empty()) {
        return true;
    }
    if (prefix.size() > target_str.size()) {
        return false;
    }
    return !target_str.compare(0, prefix.size(), prefix);
}

std::vector<std::string> Split(const std::string& target_str, const std::string& delim) {
    std::vector<std::string> elems;
    size_t pos = 0;
    size_t len = target_str.length();
    size_t delim_len = delim.length();
    if (delim_len == 0) return elems;
    while (pos < len) {
        size_t find_pos = target_str.find(delim, pos);
        if (find_pos == std::string::npos) {
            elems.push_back(target_str.substr(pos, len - pos));
            break;
        }
        elems.push_back(target_str.substr(pos, find_pos - pos));
        pos = find_pos + delim_len;
    }
  return elems;
}

std::string Join(const std::vector<std::string>& list, const std::string& separator) {
  std::stringstream ss;
  for (size_t i = 0; i < list.size(); i++) {
    ss << list[i];
    if (i != list.size() - 1) {
      ss << separator;
    }
  }
  return ss.str();
}

END_NAMESPACE_BASE_STRING
