# apple1rom
Apple1 rom making utilities for SiliconInsider/Aberco Apple1 ROMs


# Usage

makea1rom romfile.romspec romfile.bin

This will generate a 512K romefile according to the romspec file

# romfile.romspec

The format is still in flux, but looks like:

**``COPY FILE``** *``filename``* **``TO``** *``address``*.

This will copy the file at the specified address in the ROM.

*``address``* can be either a address or a *paged address*:

``1234`` : this will copy the data in any bank at address 1234
``02:1234`` : this will copy the data in bank 02 at address 1234

If the required address is not available, the program will abort with an error message.

It is possible to specify the content to copy in the romspec file:

**``COPY DATA``** *``data``* **``TO``** *``address``*.

``data`` is a list of bytes (for instance ``01020304``) and will be copied at the specified address.

Addresses can be specified as for ``COPY FILE``.

An alternative way to specify the address in both ``COPY FILE`` and ``COPY DATA`` is to use ``ANYWHERE`` instead of ``TO adrs``. The program will then find a free space in the ROM to copy the data.

``COPY FILE "file1.bin" TO ANYWHERE`` will store the content of ``file1.bin`` in the ROM at the first available address.

When a file has been copied, it can be referenced immediately in the romspec file:

``MENU something`` will define a menu item named "something". This menu will not be displayed until there are associated actions.

Actions are:

**``EXEC``** *``address``* : execute the code at the specified address. If no bank is specified, it will switch to the bank of the last file/data copied.

**``COPY``** *``address``* : copy the content of the last specified file to the specified address. If no bank is specified, it will switch to the bank of the last file/data copied. Address is a 16 bits address (RAM).

Example:

```
COPY FILE "mandelbrot65.bin" TO ANYWHERE
    MENU Mandelbrot
        COPY 0280
        EXEC 0280
```

Will copy the content of the file ``mandelbrot65.bin`` in the ROM at the first available address, then define a menu item named "Mandelbrot". When selected, it will copy the content of the file at address 0280 and execute it.
