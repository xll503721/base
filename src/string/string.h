#ifndef LB_BASE_STRING_PCH_H
#define LB_BASE_STRING_PCH_H

#include "../pch.h"

BEGIN_NAMESPACE_BASE_STRING

// trim from start (in place)
void LeftTrim(std::string& s);

// trim from end (in place)
void RightTrim(std::string& s);

// trim from both ends (in place)
void Trim(std::string& s);

std::vector<std::string> Split(const std::string& target_str, const std::string& delim);
std::string Join(const std::vector<std::string>& list, const std::string& separator);

void ReplaceAll(std::string& str, const std::string& from, const std::string& to);

bool StartsWith(const std::string& target_str, const std::string& prefix);

END_NAMESPACE_BASE_STRING

#endif /* LB_BASE_STRING_PCH_H */
