#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "options.h"

void parse_options(int argc, char *argv[], Options *options)
{
    memset(options, 0, sizeof(Options));

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-' || argv[i][1] == '\0')
        {
            continue;
        }

        if (strcmp(argv[i], "--") == 0)
        {
            break;
        }

        for (int j = 1; argv[i][j] != '\0'; j++)
        {
            switch (argv[i][j])
            {
                case 'A':
                    options->almost_all = 1;
                    break;
                case 'a':
                    options->show_all = 1;
                    break;
                case 'c':
                    options->use_ctime = 1;
                    options->use_atime = 0;
                    break;
                case 'd':
                    options->directory = 1;
                    options->recursive = 0;
                    break;
                case 'F':
                    options->classify = 1;
                    break;
                case 'f':
                    options->unsorted = 1;
                    options->show_all = 1;
                    break;
                case 'h':
                    options->human_readable = 1;
                    options->kilobytes = 0;
                    break;
                case 'i':
                    options->inode = 1;
                    break;
                case 'k':
                    options->kilobytes = 1;
                    options->human_readable = 0;
                    break;
                case 'l':
                    options->long_format = 1;
                    options->numeric_ids = 0;
                    break;
                case 'n':
                    options->numeric_ids = 1;
                    options->long_format = 0;
                    break;
                case 'q':
                    options->quote_nonprintable = 1;
                    options->raw_names = 0;
                    break;
                case 'R':
                    options->recursive = 1;
                    options->directory = 0;
                    break;
                case 'r':
                    options->reverse = 1;
                    break;
                case 'S':
                    options->sort_size = 1;
                    break;
                case 's':
                    options->blocks = 1;
                    break;
                case 't':
                    options->sort_time = 1;
                    break;
                case 'u':
                    options->use_atime = 1;
                    options->use_ctime = 0;
                    break;
                case 'w':
                    options->raw_names = 1;
                    options->quote_nonprintable = 0;
                    break;
                default:
                    fprintf(stderr, "myls: invalid option -- '%c'\n",
                            argv[i][j]);
                    exit(EXIT_FAILURE);
            }
        }
    }
}
