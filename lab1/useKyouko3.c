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

#define DEVFNAME "/dev/kyouko3"

int main(int argc, char **argv)
{
        dev_t dev = makedev(MAJOR, MINOR);

        mknod(DEVFNAME, S_IFCHR, dev);



        return 0;
}
