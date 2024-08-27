#include <stdio.h>
#include <stdlib.h>
#include <parse.h>
#include <common.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

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
    if (read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t))
    {
        perror("read");
        free(header);
        return STATUS_ERROR;
    }
    header->version = ntohs(header->version);
    header->count = ntohs(header->count);
    header->magic = ntohl(header->magic);
    header->filesize = ntohl(header->filesize);

    if (header->magic != HEADER_MAGIC)
    {
        printf("Unknown db version \n");
        free(header);
        return -1;
    }
    if (header->version != 1)
    {
        printf("Unknown db version \n");
        free(header);
        return -1;
    }

    struct stat db_stat = {0};
    fstat(fd, &db_stat);
    if (header->filesize != db_stat.st_size)
    {
        printf("Corrupted DB\n");
        free(header);
        return STATUS_ERROR;
    }

    *header_out = header;
    return STATUS_SUCCESS;
}

int read_employees(int fd, struct dbheader_t *db_header, struct employee_t **employees_out)
{
    if (fd < 0)
    {
        printf("Invalid file descriptor\n");
        return STATUS_ERROR;
    }

    uint32_t total_employees = db_header->count;
    size_t employee_list_size = total_employees * sizeof(struct employee_t);
    struct employee_t *employees = calloc(total_employees, sizeof(struct employee_t));
    if (employees == NULL)
    {
        printf("Failed to calloc employees\n");
        return STATUS_ERROR;
    }
    if (read(fd, employees, employee_list_size) != employee_list_size)
    {
        perror("read");
        free(employees);
        return STATUS_ERROR;
    }

    *employees_out = employees;
    return STATUS_SUCCESS;
}

void output_file(int fd, struct dbheader_t *header, struct employee_t *employees)
{
    if (fd < 0)
    {
        printf("Invalid file descriptor\n");
        return;
    }
    int real_count = header->count;
    header->version = htons(header->version);
    header->count = htons(header->count);
    header->magic = htonl(header->magic);
    header->filesize = htonl(sizeof(struct dbheader_t) + real_count * sizeof(struct employee_t));

    lseek(fd, 0, SEEK_SET);
    write(fd, header, sizeof(struct dbheader_t));

    for (int i = 0; i < real_count; i++)
    {
        employees[i].hours = htonl(employees[i].hours);
        write(fd, &employees[i], sizeof(struct employee_t));
    }
    return;
}

int add_employee(struct dbheader_t *header, struct employee_t *employees, char *add_string)
{
    char *name = strtok(add_string, ",");
    char *address = strtok(NULL, ",");
    char *hours = strtok(NULL, ",");

    strncpy(employees[header->count - 1].name, name, sizeof(employees[header->count - 1].name));
    strncpy(employees[header->count - 1].address, address, sizeof(employees[header->count - 1].address));
    employees[header->count - 1].hours = atoi(hours);

    return STATUS_SUCCESS;
}
