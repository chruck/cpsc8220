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

char *dev = "/dev/kyouko3";

int main(int argc, char **argv)
{
        mknod(&dev, mode, dev);



        return 0;
}
