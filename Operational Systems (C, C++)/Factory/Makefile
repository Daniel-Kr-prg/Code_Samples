EXTRA_CFLAGS ?= -fsanitize=address -fno-omit-frame-pointer

factory: factory.c
	gcc -Wall -g -O2 $(EXTRA_CFLAGS) factory.c -o factory -lpthread
