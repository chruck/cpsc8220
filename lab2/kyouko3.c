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
* Lab 2
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include "kyouko3.h"

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Mickey Mouse");

struct cdev kyouko3_cdev = {
        .owner = THIS_MODULE
};
struct pci_device_id kyouko3_dev_ids[] = {
        {PCI_DEVICE(PCI_VENDOR_ID_CCORSI, PCI_DEVICE_ID_CCORSI_KYOUKO3)},
        {0}
};

unsigned int K_READ_REG(unsigned int reg)
{
        unsigned int value = 0;
        // delay() is not needed for kyouko3, but it is on some flakey
        // hardware:
        udelay(1);
        rmb();  // read memory barrier
        // control_base is of type uint *
        //value = *(kyouko3.k_control_base + (reg >> 2));
        return value;
}

int kyouko3_release(struct inode *inode, struct file *fp)
{
        printk(KERN_ALERT "kyouko3_release()");

        return 0;
}

int kyouko3_open(struct inode *inode, struct file *fp)
{
        //ioremap (unsigned long phys_addr, unsigned long size)

        printk(KERN_ALERT "kyouko3_open()");

        return 0;
}

int kyouko3_probe(struct pci_dev *pci_dev, const struct pci_device_id *pci_id)
{
        return 0;
}

void kyouko3_remove(struct pci_dev *dev)
{
        return;
}

struct pci_driver kyouko3_pci_drv = {
        .name = "Kyouko3",
        .id_table = kyouko3_dev_ids,
        .probe = kyouko3_probe,
        .remove = kyouko3_remove,
        /*
        .node = (struct list_head)NULL,
        .suspend = (int *)NULL,
        .suspend_late = (int *)NULL,
        .resume_early = (int *)NULL,
        .resume = (int *)NULL,
        .shutdown = (void *)NULL,
        .sriov_configure = (int *)NULL,
        .err_handler = (struct pci_error_handlers *)NULL,
        .driver = (struct device_driver)NULL,
        .dynids = (struct pci_dynids)NULL,
        */
};

long kyouko3_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
        //ioctl();
        return 0;
}

int kyouko3_mmap(struct file *fp, struct vm_area_struct *vma)
{
        //mmap();

        return 0;
}

struct file_operations kyouko3_fops = {
        .open = kyouko3_open,
        .release = kyouko3_release,
        .unlocked_ioctl = kyouko3_ioctl,
        .mmap = kyouko3_mmap,
        .owner = THIS_MODULE,
};

int kyouko3_init(void)
{
        int rc = 0;

        cdev_init(&kyouko3_cdev, &kyouko3_fops);
        kyouko3_cdev.owner = THIS_MODULE;
        cdev_add(&kyouko3_cdev, MKDEV(MKNOD_MAJOR, MKNOD_MINOR), 1);

        rc = pci_register_driver(&kyouko3_pci_drv);

        printk(KERN_ALERT "kyouko3_init()");

        return rc;
}

void kyouko3_exit(void)
{
        cdev_del(&kyouko3_cdev);

        printk(KERN_ALERT "kyouko3_exit()");

        return;
}

module_init(kyouko3_init);
module_exit(kyouko3_exit);
