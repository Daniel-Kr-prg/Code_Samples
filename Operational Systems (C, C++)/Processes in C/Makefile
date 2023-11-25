all: forkpipe nsd

forkpipe: forkpipe.c
	gcc -Wall $(EXTRA_CFLAGS) forkpipe.c -o forkpipe
	
nsd: nsd_main.c nd.c nsd.c nd.h nsd.h
	gcc -Wall $(EXTRA_CFLAGS) nsd_main.c nd.c nsd.c -o nsd


