# Find the OS platform using the uname command.
Linux := $(findstring Linux, $(shell uname -s))
MacOS := $(findstring Darwin, $(shell uname -s))
Windows := $(findstring NT, $(shell uname -s))

# For native Windows only (not supported in our unit), add the .exe extension
ifdef Windows

%: %.c
	clang -std=c11 -Wall -pedantic -g $@.c -lSDL2 -o $@.exe
	./$@.exe

# For Linux/MacOS/LinuxSubSystemForWindows10, include the advanced debugging options
else

%: %.c
	clang -std=c11 -Wall -pedantic -g $@.c -lSDL2 -o $@ \
	    -fsanitize=undefined -fsanitize=address
	./$@

endif
