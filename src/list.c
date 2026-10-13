#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

#include "list.h"

typedef struct
{
    char *name;
    char *full_path;
    struct stat info;
} FileEntry;

static int compare_names(const void *a, const void *b)
{
    const FileEntry *fa = a;
    const FileEntry *fb = b;

    return strcoll(fa->name, fb->name);
}

static int compare_sizes(const void *a, const void *b)
{
    const FileEntry *fa = a;
    const FileEntry *fb = b;

    if (fa->info.st_size < fb->info.st_size)
        return 1;

    if (fa->info.st_size > fb->info.st_size)
        return -1;

    return compare_names(a, b);
}

static time_t get_file_time(const struct stat *info, const Options *options)
{
    if (options->use_ctime)
        return info->st_ctime;

    if (options->use_atime)
        return info->st_atime;

    return info->st_mtime;
}

static int compare_times(const void *a, const void *b)
{
    const FileEntry *fa = a;
    const FileEntry *fb = b;

    extern const Options *current_sort_options;

    time_t ta = get_file_time(&fa->info, current_sort_options);
    time_t tb = get_file_time(&fb->info, current_sort_options);

    if (ta < tb)
        return 1;

    if (ta > tb)
        return -1;

    return compare_names(a, b);
}

const Options *current_sort_options = NULL;

static int is_hidden(const char *name)
{
    return name[0] == '.';
}

static void print_permissions(mode_t mode)
{
    char permissions[11];

    permissions[0] = S_ISDIR(mode) ? 'd' :
                     S_ISLNK(mode) ? 'l' :
                     S_ISCHR(mode) ? 'c' :
                     S_ISBLK(mode) ? 'b' :
                     S_ISFIFO(mode) ? 'p' :
                     S_ISSOCK(mode) ? 's' : '-';

    permissions[1] = mode & S_IRUSR ? 'r' : '-';
    permissions[2] = mode & S_IWUSR ? 'w' : '-';
    permissions[3] = mode & S_IXUSR ? 'x' : '-';
    permissions[4] = mode & S_IRGRP ? 'r' : '-';
    permissions[5] = mode & S_IWGRP ? 'w' : '-';
    permissions[6] = mode & S_IXGRP ? 'x' : '-';
    permissions[7] = mode & S_IROTH ? 'r' : '-';
    permissions[8] = mode & S_IWOTH ? 'w' : '-';
    permissions[9] = mode & S_IXOTH ? 'x' : '-';
    permissions[10] = '\0';

    if (mode & S_ISUID)
        permissions[3] = (mode & S_IXUSR) ? 's' : 'S';

    if (mode & S_ISGID)
        permissions[6] = (mode & S_IXGRP) ? 's' : 'S';

    if (mode & S_ISVTX)
        permissions[9] = (mode & S_IXOTH) ? 't' : 'T';

    printf("%s", permissions);
}

static void print_size(off_t size, const Options *options)
{
    if (options->human_readable)
    {
        double value = (double)size;
        const char *units[] = {"B", "K", "M", "G", "T", "P"};
        int unit = 0;

        while (value >= 1024.0 && unit < 5)
        {
            value /= 1024.0;
            unit++;
        }

        printf("%7.1f%s ", value, units[unit]);
    }
    else
    {
        printf("%8lld ", (long long)size);
    }
}

static void print_name(const char *name, const Options *options)
{
    const unsigned char *p = (const unsigned char *)name;

    while (*p)
    {
        if (options->raw_names || (*p >= 32 && *p != 127))
        {
            putchar(*p);
        }
        else if (options->quote_nonprintable)
        {
            putchar('?');
        }
        else
        {
            putchar(*p);
        }

        p++;
    }
}

static void print_entry(const FileEntry *entry, const Options *options)
{
    const struct stat *info = &entry->info;

    if (options->inode)
        printf("%8ju ", (uintmax_t)info->st_ino);

    if (options->blocks)
    {
        unsigned long long blocks = (unsigned long long)info->st_blocks;

        if (options->human_readable)
            printf("%4llu ", blocks);
        else if (options->kilobytes)
            printf("%4llu ", (blocks + 1) / 2);
        else
            printf("%4llu ", blocks);
    }

    if (options->long_format || options->numeric_ids)
    {
        print_permissions(info->st_mode);
        printf(" %2lu ", (unsigned long)info->st_nlink);

        if (options->numeric_ids)
        {
            printf("%-8u %-8u ",
                   (unsigned int)info->st_uid,
                   (unsigned int)info->st_gid);
        }
        else
        {
            struct passwd *pw = getpwuid(info->st_uid);
            struct group *gr = getgrgid(info->st_gid);

            printf("%-8s %-8s ",
                   pw ? pw->pw_name : "?",
                   gr ? gr->gr_name : "?");
        }

        print_size(info->st_size, options);

        time_t file_time = get_file_time(info, options);
        char time_buffer[32];
        struct tm *time_info = localtime(&file_time);

        if (time_info != NULL)
            strftime(time_buffer, sizeof(time_buffer),
                     "%b %e %H:%M", time_info);
        else
            snprintf(time_buffer, sizeof(time_buffer), "???");

        printf("%s ", time_buffer);
    }

    print_name(entry->name, options);

    if (options->classify)
    {
        if (S_ISDIR(info->st_mode))
            putchar('/');
        else if (S_ISLNK(info->st_mode))
            putchar('@');
        else if (S_ISFIFO(info->st_mode))
            putchar('|');
        else if (S_ISSOCK(info->st_mode))
            putchar('=');
        else if (info->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
            putchar('*');
    }

    if ((options->long_format || options->numeric_ids) &&
        S_ISLNK(info->st_mode))
    {
        char target[4096];

        ssize_t length = readlink(
            entry->full_path, target, sizeof(target) - 1);

        if (length >= 0)
        {
            target[length] = '\0';
            printf(" -> %s", target);
        }
    }

    putchar('\n');
}

static void free_entries(FileEntry *entries, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        free(entries[i].name);
        free(entries[i].full_path);
    }

    free(entries);
}

static int add_entry(
    FileEntry **entries,
    size_t *count,
    size_t *capacity,
    const char *directory,
    const char *name)
{
    if (*count == *capacity)
    {
        size_t new_capacity = (*capacity == 0) ? 16 : *capacity * 2;

        if (new_capacity < *capacity ||
            new_capacity > SIZE_MAX / sizeof(FileEntry))
        {
            errno = ENOMEM;
            return -1;
        }

        FileEntry *new_entries = realloc(
            *entries, new_capacity * sizeof(FileEntry));

        if (new_entries == NULL)
            return -1;

        *entries = new_entries;
        *capacity = new_capacity;
    }

    size_t path_length = strlen(directory) + strlen(name) + 2;
    char *full_path = malloc(path_length);

    if (full_path == NULL)
        return -1;

    snprintf(full_path, path_length, "%s/%s", directory, name);

    struct stat info;

    if (lstat(full_path, &info) != 0)
    {
        perror(full_path);
        free(full_path);
        return 1;
    }

    char *name_copy = strdup(name);

    if (name_copy == NULL)
    {
        free(full_path);
        return -1;
    }

    (*entries)[*count].name = name_copy;
    (*entries)[*count].full_path = full_path;
    (*entries)[*count].info = info;

    (*count)++;

    return 0;
}

static int should_show(const char *name, const Options *options)
{
    if (options->almost_all)
        return strcmp(name, ".") != 0 &&
               strcmp(name, "..") != 0;

    if (!options->show_all && is_hidden(name))
        return 0;

    return 1;
}

static void sort_entries(
    FileEntry *entries,
    size_t count,
    const Options *options)
{
    if (options->unsorted)
        return;

    if (options->sort_time)
    {
        current_sort_options = options;
        qsort(entries, count, sizeof(FileEntry), compare_times);
    }
    else if (options->sort_size)
    {
        qsort(entries, count, sizeof(FileEntry), compare_sizes);
    }
    else
    {
        qsort(entries, count, sizeof(FileEntry), compare_names);
    }

    if (options->reverse)
    {
        for (size_t i = 0; i < count / 2; i++)
        {
            FileEntry temp = entries[i];
            entries[i] = entries[count - i - 1];
            entries[count - i - 1] = temp;
        }
    }
}

static void list_directory_internal(
    const char *path,
    const Options *options,
    int recursive)
{
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        perror(path);
        return;
    }

    FileEntry *entries = NULL;
    size_t count = 0;
    size_t capacity = 0;

    struct dirent *item;

    while ((item = readdir(dir)) != NULL)
    {
        if (!should_show(item->d_name, options))
            continue;

        int result = add_entry(
            &entries, &count, &capacity, path, item->d_name);

        if (result < 0)
        {
            perror("myls: allocate memory");
            free_entries(entries, count);
            closedir(dir);
            return;
        }
    }

    closedir(dir);

    sort_entries(entries, count, options);

    if (recursive)
        printf("%s:\n", path);

    for (size_t i = 0; i < count; i++)
        print_entry(&entries[i], options);

    if (recursive && count > 0)
        putchar('\n');

    if (recursive)
    {
        for (size_t i = 0; i < count; i++)
        {
            if (S_ISDIR(entries[i].info.st_mode) &&
                strcmp(entries[i].name, ".") != 0 &&
                strcmp(entries[i].name, "..") != 0)
            {
                list_directory_internal(
                    entries[i].full_path, options, 1);
            }
        }
    }

    free_entries(entries, count);
}

void list_directory(const char *path, const Options *options)
{
    list_directory_internal(path, options, options->recursive);
}

void list_path(const char *path, const Options *options)
{
    struct stat info;

    if (lstat(path, &info) != 0)
    {
        perror(path);
        return;
    }

    if (S_ISDIR(info.st_mode) && !options->directory)
    {
        list_directory(path, options);
    }
    else
    {
        FileEntry entry;

        entry.name = (char *)path;
        entry.full_path = (char *)path;
        entry.info = info;

        print_entry(&entry, options);
    }
}
