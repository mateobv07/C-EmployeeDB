#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include "file.h"
#include "common.h"
#include "parse.h"

void print_usage(char *argv[])
{
    printf("%s <filename>\n", argv[0]);
}

int main(int argc, char *argv[])
{
    char *filepath = NULL;
    char *addstring = NULL;
    bool newfile = false;
    bool read_db = false;
    int c;

    int db_fd = -1;
    struct dbheader_t *header = NULL;
    struct employee_t *employees = NULL;

    while ((c = getopt(argc, argv, "nf:a:l")) != -1)
    {
        switch (c)
        {
        case 'n':
            newfile = true;
            break;
        case 'f':
            filepath = optarg;
            break;
        case 'a':
            addstring = optarg;
            break;
        case 'l':
            read_db = true;
            break;
        case '?':
            printf("Unknown option -%c\n", c);
            break;
        default:
            return -1;
        }
    }

    if (filepath == NULL)
    {
        printf("File path is a required argument\n");
        print_usage(argv);
        return 0;
    }

    if (newfile)
    {
        db_fd = create_db_file(filepath);
        if (db_fd == STATUS_ERROR)
        {
            printf("Unable to create database file\n");
            return -1;
        }
        if (create_db_header(db_fd, &header) == STATUS_ERROR)
        {
            printf("Unable to create database header\n");
            return -1;
        }
    }
    else
    {
        db_fd = open_db_file(filepath);
        if (db_fd == STATUS_ERROR)
        {
            printf("Unable to open database file\n");
            return -1;
        }
        if (validate_db_header(db_fd, &header) == STATUS_ERROR)
        {
            printf("Failed to validate database header\n");
            return -1;
        }
    }
    if (read_employees(db_fd, header, &employees) != STATUS_SUCCESS)
    {
        printf("Failed to read employees");
    }
    if (addstring != NULL)
    {
        header->count++;
        employees = realloc(employees, header->count * sizeof(struct employee_t));
        add_employee(header, employees, addstring);
    }

    if (read_db)
    {
        for (int i = 0; i < header->count; i++)
        {
            printf("Employee: %d\n", i + 1);
            printf("-- Name: %s\n", employees[i].name);
            printf("-- Address: %s\n", employees[i].address);
        }
    }
    output_file(db_fd, header, employees);

    // FREE MEMORY?
    free(header);
    free(employees);
    return 0;
}
