// #include <unistd.h> /* TODO: replace this by writing your own system call */
                    /* wrappers for read(), write(), exit() */
// #include <stdio.h>  /* TODO: replace this by your own implementation of */
                    /* sprintf() (for conversion of a number to hex string) */
// #include <string.h> /* TODO: replace this with your implementation of strlen() */

#define STDIN_FILENO 0
#define STDOUT_FILENO 1

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4

void exit(int ext_val)
{
    asm volatile("int $0x80"
    : : "a"(SYS_EXIT), "b"(ext_val));
}
int read(int fd, void *buffer, int length)
{
    int res;
    asm volatile("int $0x80"
    : "=a"(res)
    : "a"(SYS_READ), "b"(fd), "c"(buffer), "d"(length));
    return res;
}
int write(int fd, void *buffer, int length)
{
    int res;
    asm volatile("int $0x80"
    : "=a"(res)
    : "a"(SYS_WRITE), "b"(fd), "c"(buffer), "d"(length));
    return res;
}

int isnum(char ch)
{
    return ch >= '0' && ch <= '9';
}

int isspc(char ch)
{
    return ch == ' ' || ch == '\n';
}

void num_to_hex_str(char * buf, unsigned num)
{
    int i = 0;
    int totalSize = 0;
    char tmpbuf[20];
    char *nums = "0123456789abcdef";
    do
    {
        tmpbuf[i] = nums[num % 16];
        i++;
        totalSize++;
        num = num / 16;
    } while (num > 0);
    i--;
    do
    {
        buf[totalSize - i + 1] = tmpbuf[i];
        i -= 1;
    } while (i >= 0);

    buf[totalSize + 2] = '\n';
    buf[0] = '0';
    buf[1] = 'x';
}

int strlen(char * str)
{
    int len = 0;
    char ch = 1;
    while (ch != '\n')
    {
        ch = str[len++];
    }
    return len;
}

static void print(unsigned num)
{
    char buf[20];
    num_to_hex_str(buf, num);
    int ret = write(STDOUT_FILENO, buf, strlen(buf));
    if (ret == -1)
        exit(1);
}

/* TODO: main() is called by libc. Without libc, the entry point is called _start(). */
int main()
{
    char buf[20];
    unsigned num = 0;
    int i;
    int num_digits = 0;
    unsigned chars_to_process = 0;

    for (/* no init */; /* no end condition */; i++, chars_to_process--) {
        if (chars_to_process == 0) {
            int ret = read(STDIN_FILENO, buf, sizeof(buf));
            if (ret < 0)
                exit(1);
            i = 0;
            chars_to_process = ret;
        }
        if (
            num_digits > 0
            && (chars_to_process == 0 /* EOF */ || !isnum(buf[i]))
        ) {
            print(num);
            num_digits = 0;
            num = 0;
        }
        if (
            chars_to_process == 0 /* EOF */
            || (!isspc(buf[i]) && !isnum(buf[i]))
        )
            exit(0);

        if (isnum(buf[i])) {
            num = num * 10 + buf[i] - '0';
            num_digits++;
        }
    }
}

void _start()
{
    main();
}