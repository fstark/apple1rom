#include <stdio.h>
#include <string>
#include "parser.h"
#include "mapping.h"
#include "rom512.h"

int main( int argc, const char **argv )
{
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    /// Parse entry specs
    auto entries = parse_romentryspecs(argv[1]);

    rom512 rom;
    mapping map(rom);

    for (const auto& entry : entries) {
        map.add_entry(entry);
    }

    return 0;
}
