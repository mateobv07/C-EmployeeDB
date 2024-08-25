#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
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
    bool newfile = false;
    int c;

    int db_fd = -1;
    struct dbheader_t *header = NULL;

    while ((c = getopt(argc, argv, "nf:")) != -1)
    {
        switch (c)
        {
        case 'n':
            newfile = true;
            break;
        case 'f':
            filepath = optarg;
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
    }

    return 0;
}
