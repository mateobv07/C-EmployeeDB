#pragma once
#include <stdint.h>
#define HEADER_MAGIC 0x4c4c4144

struct dbheader_t
{
    uint32_t magic;
    uint16_t version;
    uint16_t count;
    uint32_t filesize;
};

struct employee_t
{
    char name[256];
    char address[256];
    uint32_t hours;
};

int create_db_header(int fd, struct dbheader_t **header_out);
int validate_db_header(int fd, struct dbheader_t **header_out);
int read_employees(int fd, struct dbheader_t *, struct employee_t **employees_out);
int add_employee(struct dbheader_t *, struct employee_t *, char *add_string);
void output_file(int fd, struct dbheader_t *, struct employee_t *);