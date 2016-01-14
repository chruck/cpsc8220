/**
* @file kyouko3.c
* @author Jas Eckard <eckard@clemson.edu>
*
* @section LICENSE
*
* Proprietary
*
* @section DESCRIPTION
*
* Lab 1
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include "kyouko3.h"

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Mickey Mouse");

int kyouko3_release(struct inode *inode, struct file *fp)
{
        printk(KERN_ALERT "kyouko3_release()");

        return 0;
}

int kyouko3_open(struct inode *inode, struct file *fp)
{
        printk(KERN_ALERT "kyouko3_open()");

        return 0;
}

struct file_operations kyouko3_fops = {
        .open = kyouko3_open,
        .release = kyouko3_release,
        .owner = THIS_MODULE
};
struct cdev whatever;

int kyouko3_init(void)
{
        cdev_init(&whatever, &kyouko3_fops);
        cdev_add(&whatever, MKDEV(MKNOD_MAJOR, MKNOD_MINOR), 1);

        printk(KERN_ALERT "kyouko3_init");

        return 0;
}

int kyouko3_exit(void)
{
        cdev_del(&whatever);

        printk(KERN_ALERT "kyouko3_exit");

        return 0;
}

module_init(kyouko3_init);
module_exit(kyouko3_exit);
