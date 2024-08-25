#include <stdio.h>
#include <parse.h>
#include <common.h>

int create_db_header(int fd, struct dbheader_t **header_out)
{
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == NULL)
    {
        printf("Malloc failed to create db header\n");
        return STATUS_ERROR;
    }
    header->version = 1;
    header->count = 0;
    header->magic = HEADER_MAGIC;
    header->filesize = sizeof(struct dbheader_t);

    *header_out = header;
    return STATUS_SUCCESS;
};

int validate_db_header(int fd, struct dbheader_t **header_out)
{
    if (fd < 0)
    {
        printf("Invalid file descriptor\n");
        return STATUS_ERROR;
    }
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == NULL)
    {
        printf("Malloc failed to create db header\n");
        return STATUS_ERROR;
    }
}