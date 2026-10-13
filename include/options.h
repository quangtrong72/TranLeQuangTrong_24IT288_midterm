#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct {
    int show_all;
    int almost_all;
    int directory;
    int classify;
    int human_readable;
    int inode;
    int long_format;
    int numeric_ids;
    int recursive;
    int reverse;
    int sort_size;
    int sort_time;
    int unsorted;
    int blocks;
    int kilobytes;
    int use_ctime;
    int use_atime;
    int quote_nonprintable;
    int raw_names;
} Options;

void parse_options(int argc, char *argv[], Options *options);

#endif
