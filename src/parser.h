#pragma once

#include <string>
#include <vector>
#include "romentryspec.h"

/// Parses a file containing ROM entry specifications. 
std::vector<romentryspec> parse_romentryspecs(const std::string& filename);
