#pragma once

#include <memory>
#include <string>
#include <vector>

#include "romentryspec.hpp"

/// Parses a file containing ROM entry specifications.
std::vector<std::shared_ptr<romentryspec>> parse_romentryspecs(const std::string& filename);
