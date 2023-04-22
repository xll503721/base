#ifndef LB_BASE_STRING_PCH_H
#define LB_BASE_STRING_PCH_H

BEGIN_NAMESPACE_LBBASE

// trim from start (in place)
void LeftTrim(std::string& s);

// trim from end (in place)
void RightTrim(std::string& s);

// trim from both ends (in place)
void Trim(std::string& s);

END_NAMESPACE_LBBASE
