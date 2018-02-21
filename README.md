# BlackDOS Project, Spring 2018
## Sam Borick

### Currently, this OS can:

1. Accept string or integer input
2. Print strings or integers
3. Read data from the floppy device
4. Write data to the floppy device

Currently on startup the OS sets a color scheme (currently l33t HAXOR), then reads an ASCII image of a cat from the disk to the screen, and writes a simple config file onto the floppy.

This can be tested by running compileOS.sh.  The result should run in the boch emulator.

### Notes:

- (Resolved?) Currently the OS has been observed to crash on startup.  This occurs sporadically, and when run again (rarely after as many as 3 attempts) the machine boots as expected.  The cause of this is currently unknown.

- compileOS.sh attempts to copy the contents of the config after bochs exits, however this command will not be run if force exited.