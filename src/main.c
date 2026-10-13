#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "options.h"
#include "list.h"

int main(int argc, char *argv[])
{
    Options options;
    parse_options(argc, argv, &options);

    int path_count = 0;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-' || argv[i][1] == '\0')
        {
            path_count++;
        }
    }

    if (path_count == 0)
    {
        list_path(".", &options);
        return EXIT_SUCCESS;
    }

    int exit_status = EXIT_SUCCESS;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && argv[i][1] != '\0')
            continue;

        list_path(argv[i], &options);
    }

    return exit_status;
}
