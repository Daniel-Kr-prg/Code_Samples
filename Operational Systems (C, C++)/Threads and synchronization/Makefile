EXTRA_CFLAGS ?= -fsanitize=address -fno-omit-frame-pointer

prod-cons: prod-cons.c
	gcc -Wall -g -O2 $(EXTRA_CFLAGS) prod-cons.c -o prod-cons -lpthread
	
