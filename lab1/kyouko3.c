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

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Mickey Mouse");

int kyouko3_release(struct inode *inode, struct file *fp)
{
        printk(KERNALERT "kyouko3_release()");

        return 0;
}

int kyouko3_open(struct inode *inode, struct file *fp)
{
        printk(KERNALERT "kyouko3_open()");

        return 0;
}

struct file_operations kyouko3_fops = {
        .open = kyouko3_open,
        .release = kyouko3_release,
        .owner = THIS_MODULE
};

/*
module_init(kyouko3_init);

struct cdev whatever;

cdev_init(&whatever, &kyouko3_fops);
cdev_add(&whatever, MKDEV(major, minor), 1);

printk(KERNALERT "whatever");

return 0;

module_exit(kyouko3_exit);

cdev_del();
printk(KERNALERT "whatever");
*/
