#include <stdio.h>
#include <string>
#include "parser.h"

int main( int argc, const char **argv )
{
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    /// Parse entry specs
    std::vector<romentryspec> entries = parse_romentryspecs(argv[1]);
    return 0;
}
