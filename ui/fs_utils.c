#define _XOPEN_SOURCE 500 

#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>

#include "fs_utils.h"

int rmFiles(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    if(remove(pathname) < 0)
    {
        perror("ERROR: remove");
        return -1;
    }
    return 0;
}

void remove_dir_recursive(char *path)
{
    printf("removing %s\n", path);
    if (nftw(path, rmFiles,10, FTW_DEPTH|FTW_MOUNT) < 0)
    {
        perror("ERROR: ntfw");
        exit(1);
    }
}