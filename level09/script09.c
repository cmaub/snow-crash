#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 2)
        return -1;

    int fd = open(argv[1], O_RDONLY, O_WRONLY);
    if (!fd)
        return -1;
    
    char str[1024];
    int size = read(fd, str, 1024);
    if (size < 0)
    {
        close(fd);
        return -1;
    }
    
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        
        if (i == 0)
            str[i] = str[i];
        else
            str[i] = str[i] - count;
        count++;
    }
    printf("%s\n", str);
    close (fd);
    return 0;
}