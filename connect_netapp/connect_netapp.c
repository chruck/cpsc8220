//
// connect_netapp.c - a serial port connection routine used to
// configure NetApp filers, e.g., the FAS960c.
//

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define INCOMING 3
#define OUTGOING 4
#define FOREVER for(;;)

struct fax {
        char *portname;
        char *tty;
        char *lock;
        unsigned short speed;
        unsigned short size;
        unsigned short mode;
        unsigned short startstop;
        unsigned short turnoff;
};

// Global:
struct fax comstuff[] = {
        "blueshark", "/dev/ttyS0", "/var/tmp/LOCKPORT", B9600, CS8,
        CREAD, IXOFF, (ECHO|ICANON),
        "bluefish", "/dev/ttyS1", "/var/tmp/LOCKPORT", B9600, CS8,
        CREAD, IXOFF, (ECHO|ICANON),
        0
};

// read INCOMING, write to STDOUT
void incoming()
{
        char buf;

        FOREVER
                while (read(INCOMING, &buf, 1))
                        write(STDOUT, &buf, 1);
}

// read STDIN, write to OUTGOING until '#'
void outgoing()
{
        char buf;

        FOREVER
                while (read(STDIN, &buf, 1)) {
                        // What's a good escape character?
                        if ('#' == buf)
                                return;
                        write(OUTGOING, &buf, 1);
                }
}

// Usage
void commanderror()
{
        struct fax *ptr;

        printf("syntax:  connect <portname>\n");
        printf("portnames available are:\n");

        ptr = comstuff;

        while (ptr->portname) {
                printf("%s\n", ptr->portname);
                ++ptr;
        }

        exit(1);
}

int main(int argc, char **argv)
{
        int pid, status;
        struct termio externalport, controlport, saveexternal,
                      savecontrol;
        struct fax *ptr;

        // 2 args req'd --> Usage
        if (2 != argc)
                commanderror();  // this is an exit

        ptr = comstuff;

        while (ptr->portname) {
                // Match 'blueshark', 'bluefish'
                if (0 == strcmp(ptr->portname, argv[1]))
                        break;
                ++ptr;
        }

        // End of Global's list:  Usage
        if (0 == ptr->portname)
                commanderror();

        if (0 <= access(ptr->lock, F_OK)) {
                printf("port in use\n");
                exit(1);
        }

        if (0 > close(creat(ptr->lock, 0))) {
                printf("system bug:\n");
                printf("cannot lock port\n");
                exit(1);
        }

        if (INCOMING != open(ptr->tty, 0)) {
                printf("cannot open %s for read\n", ptr->tty);
                unlink(ptr->lock);
                exit(1);
        }

        if (OUTGOING != open(ptr->tty, 1)) {
                printf("cannot open %s for write\n", ptr->tty);
                unlink(ptr->lock);
                exit(1);
        }

        // set control terminal to raw mode
        ioctl(STDIN, TCGETA, &savecontrol);

        controlport = savecontrol;

        controlport.c_cflag &= ~(PARENB | CSIZE);
        controlport.c_cflag |= CS8;
        controlport.c_lflag &= ~(ECHO | ICANON | ISIG);
        controlport.c_iflag |= IGNBRK;
        controlport.c_iflag &= ~(ISTRIP | INLCR | IGNCR | ICRNL);

        ioctl(STDOUT, TCSETA, &controlport);
        ioctl(STDIN, TCSETA, &controlport);

        // set io port
        ioctl(OUTGOING, TCGETA, &saveexternal);

        externalport = saveexternal;

        externalport.c_cflag = ptr->speed;
        externalport.c_cflag |= ptr->size;
        externalport.c_cflag |= ptr->mode;
        externalport.c_lflag &= ~(ptr->turnoff);
        externalport.c_iflag = ptr->startstop;

        ioctl(OUTGOING, TCSETA, &externalport);
        ioctl(INCOMING, TCSETA, &externalport);

        switch (pid = fork()) {
                case -1:
                        printf("OOPS - no fork!\n");
                        exit(1);

                case 0:  // child
                        incoming();
                        break;

                default:  // parent
                        outgoing();
                        kill(pid, SIGKILL);  // kill the kid
                        while (wait(&status) != pid);

                        // restore settings
                        ioctl(STDIN, TCSETA, &savecontrol);
                        ioctl(STDOUT, TCSETA, &savecontrol);
                        ioctl(INCOMING, TCSETA, &saveexternal);
                        ioctl(OUTGOING, TCSETA, &saveexternal);

                        unlink(ptr->lock);

                        break;
        }

        return 0;
}
