#pragma once

#include <string>
#include <vector>
#include <memory>
#include "romentryspec.h"

/// Parses a file containing ROM entry specifications. 
std::vector<std::shared_ptr<romentryspec>> parse_romentryspecs(const std::string& filename);
