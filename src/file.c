#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"

int create_db_file(char *file_name)
{
    // Check if file already exists
    int fd = open(file_name, O_RDWR);
    if (fd != -1)
    {
        close(fd);
        printf("File already exists\n");
        return STATUS_ERROR;
    }

    fd = open(file_name, O_RDWR | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("open");
        return STATUS_ERROR;
    }

    return fd;
}

int open_db_file(char *file_name)
{
    int fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("open");
        return STATUS_ERROR;
    }

    return fd;
}