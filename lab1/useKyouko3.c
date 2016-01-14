/**
* @file useKyouko3.c
* @author Jas Eckard <eckard@clemson.edu>
*
* @section LICENSE
*
* (GPL license?)
*
* @section DESCRIPTION
*
* This file is ...
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "kyouko3.h"

int main(int argc, char **argv)
{
        dev_t dev = makedev(MKNOD_MAJOR, MKNOD_MINOR);

        mknod(DEVFNAME, S_IFCHR, dev);

        int fd = open(DEVFNAME, O_RDWR);

        close(fd);

        return 0;
}
