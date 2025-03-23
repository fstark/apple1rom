# apple1rom
Apple1 rom making utilities


# Usage

makea1rom romfile.romspec

# romfile.romspec

list of files to be present in rom

directory/name binary <binfile> <address>

loads binfile in the rom. adds an entry to it.
it will be copied at address when started

directory/name romcode <binfile> <address>

loads binfile in the rom. adds an entry to it.
the binfile is loaded at the specified address in the ROM and exexcuted in place

<!-- directory/name basic <binfile> <address> -->


