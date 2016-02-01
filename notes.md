<!-- vim: set filetype=markdown : -->

# CPSC 8220:  Case Study in Operating Systems
and
# CPSC 8221:  Case Study in Operating Systems Lab

`Jan 7, 2016`

Instructors:
- Robert Geist
- Aubrey Lawson - lab TA

Times:
- TTh 15:45 - 16:35 - class
- W 17:00 - 18:50 - lab: 110D

Content:  How a real operating system works -> Linux 4.3.3

No documentation except source code

Book - Linux Device Drivers 4th ed -> 3.2 kernel -> coming out

## Reasons to take the course
1. job in systems software
2. understand system performance

### NOT
1. satisfy PhD requirement
2. boost GPA

## Text
* (class notes & handouts)
* R. Love: Linux Kernel Development 3rd ed. (2.6 kernel)

## Grades
- 3 Programming Projects -> Teams 15% each

1. write device driver for graphics card -> IRQs, memory management
2. SCSI device?
3. ?

- Midterm exam - March 8 - 20%
- Final exam - Apr 29 - 25%
  - both open book/notes/laptop
- Lab - 10%

Scale  | Grade
------ | -----
85-100 | A
70-84  | B
55-69  | C
54-0   | F

## Office Hours
308 McAdams, 15:00-17:00 MWF (but come anytime)

## Attendance
- "reservation system"
- not required -> no roll
- all tests, projects, hints, etc are class notes
- no individual lectures for missed material
- no handouts

## Prereq
1. OS Principles (lock hierarchy, race conditions, semaphores)
2. ability to read/write non-trivial C code

- Running in VM - KVM
- Graphics Card - qemu
- QEMU - generic hardware
  - modified for real-looking like real AMD

## Section 0: Warm-up Exercise
(not part of kernel: get used to system calls)

(handout)
- user-level C code to review
  - provides serial line control
  - actually used to low-level config NetApp filers
- to observe
1. level of C sophistication
  minimum -> starting point
2. examples of important system calls
  - `fork()`
  - `wait()`
  - `ioctl()` - going to implement a lot of these

### BASIC DESIGN

                         +--------------------+
                         |                    |
      +---------+        |-+ <- (child) <-- +-|        +--------------+
      |         |        | |                | |        |              |
      | monitor |--------| |                | |--------| NetApp filer |
      |         |    /-> | |                | | <-\    | (FAS960c)    |
      +---------+   /    |-+ -> (parent) -> +-|    \   |              |
     / keyboard /  /     |                    |     \  +--------------+
    /----------/  /      +--------------------+      \
                control port       ^          external port (serial line)
                                   |
                           launch 2 processes:

- [x] show me
  - Geist wrote to write over code in class
  - email request code for
  - draws on top of ASCII text files
  - needs touch-sensitive display

**-->** Don't have time for error checking in this course, minimal error
checking in our code is sufficient

`Jan 12`

- CJ Coursey - NetApp employee and student
  - ccorsi@clemson.edu
  - wrote the "AMD" graphics card

## VMs in the lab tomorrow:
- `130.127.106.40`
- `ssh eckard@130.127.106.40`
  - passwd ~~cpsc123~~
- then, `https://130.127.106.40`
  - accept certificate error
  - Click `User Portal`
  - user/passwd
  - domain: `plab`
  - 10 minute boot menu
  - choose `kernel 4.3.3`
- After the first lab, the video card will be in the VM
- CentOS

### Lab 1
- VM bring up - modify kernel
- kernel module

## Warmup code

                         +--------------------+
                         |                    |
      +---------+        |-+ <- (child) <-- +-|        +--------------+
      |         |        | |                | |        |              |
      | monitor |--------| |                | |--------| NetApp filer |
      |         |    /-> | |                | | <-\    | (FAS960c)    |
      +---------+   /    |-+ -> (parent) -> +-|    \   |              |
     / keyboard /  /     |                    |     \  +--------------+
    /----------/  /      +--------------------+      \
                control port       ^          external port (serial line)
                                   |
                           launch 2 processes:

- Ctrl-C -> kills control program
- double echo -> good ports echo what typed
- error code -> echo back to filer -> endless loop
- remove echoing on one of
- don't interpret special code

_(File descriptor numbers from above diagram:)_

    +------------------------+
    | (stdin)                |
    |-0-+ <- (child) <-- +-3-|
    |   |                |   |
    |   |                |   |
    |   |                |   |
    |-1-+ -> (parent) -> +-4-|
    | (stdout)               |
    +------------------------+

lab:  kernel module

`Jan 13`

## Lab 1
module:
- module init \\\_ macros/functions
- module exit /

- `/sbin/insmod mymod.ko`  <- kernel object
- `/sbin/rmmod mymod`  <- no '`.ko`'
- `/sbin/lsmod`

On load: `printk()` from kernel
- `printk()` needs leading qualifier:  priority (0-7: lower is more urgent)
- console priority - messages recorded or not

`Jan 14`

OVirt OpenVZ KVM QEmu

"KVM and RAID will disappear"

## Section 1:  Module-based Device Drivers
- Modules offer quick kernel modifications - dynamically
- vs. "old way" - compile/reboot -> more convenient

K&R - version 6 - mysterious variable with '`++`'
- Ken coding in one direction, Dennis decrements.  Where their code
joins, there is a '`++`'.

Linux - complains of security of modules

Convenient, except:
- To be useful, modues need access to kernel symbols - variables,
        functions, structs
- Only those symbols that have been exported for module use are
        available
- Older -> symbols not available
  - `/proc/kallsyms` <- shows all symbols in kernel
  - `linux/Module.symvers` <- shows those exported
  - if exported symbols don't contain needed then modify kernel,
        recompile, reboot.
  - to export:  use macro `EXPORT_SYMBOL()`

Using modules _can_ cost more time (initially)

Use `nm module.ko | grep --ksymtab` to see symbols exported by `module.ko`

### Device Driver
Simple, in principle:
- device is usually a card inserted into a PCIe slot, after which the
device's onboard command registers appear at fixed memory addresses
- the driver only needs to load these registers with bit strings that
are commands understood by the device
- the bits:  from the manual of the device -> registers and what the
control.  AKA 'data book' - hidden, unless you work for a large
company

The card we have is very close to an ATI, which its databook.

_In practice_:
- Linux uses paged virtual memory
  - so, there are 3 address spaces:
    - physical
    - kernel virtual
    - user virtual
  - the kernel and user virtual oddresses map onto physical
  - older systems:  ~1/4 kernel virtual, ~3/4 user virtual
  - and, 2 sets of page tables to get through:
  - further, most devices use DMA (Direct Memory Access), where the
  card reaches into main memory (via the physical address)
    - DMA buffer managemnte is necessary (for us)
    - card is independant of the kernel, so race conditions need to be
    accounted for
    - when it is done with the memory, it sends an interrupt
    - so, an interrupt handler is needed

A convenient vehicle for driver design is the Unix _character device_
structure.
- character devices support the `ioctl()` system call for which one
arguement (the second one) is a 'command'.
Therefore, you can have many cards in one with a large `switch`
statement.

"`ioctl()` is a Swiss Army Knife."

Also, (besides driver code) we need to implement user code -> group
with others

Implement these 6 functions:
1. `module_init()`
2. `module_exit()`
3. `kyouto3_open()`
4. `kyouto3_release()`
5. `kyouto3_mmap()`
6. `kyouto3_ioctl()`

Use tagged initialization:

_(kernel source)_

    struct file_operations kyouto3_fops = {
            .open = kyouto3_open,
            .release = kyouto3_release,
            // emphasis on "unlocked", because kernel 2.6 and before
            // had the "big kernel lock"
            .unlocked_ioctl = kyouto3_ioctl,
            .mmap = kyouto3_mmap,
            .owner = THIS_MODULE  // keeps a usage count
    };

From `<cdev.h>`:

    struct cdev kyouto3_cdev

Thus, a user-level call to:
- `open()` calls to `kyouto3_open()`
- `close()` calls to `kyouto3_release()`
- `ioctl()` calls to `kyouto3_ioctl()`
- `running()` calls and gets results from `kyouto3_mmap()`

`module_init()`:

    cdev_init(&kyouto3_cdev, &kyouto3_fops);
    kyouto3_cdev.owner = THIS_MODULE;
    // cdev address:
    cdev_add(&kyouto3_cdev, MKDEV(maj, min), 1);

where `maj` and `min` are `KYOUKO3_MAJOR` and `KYOUKO3_MINOR`, respectively

Still need to scan the PCIe bus to find the correct one -> not the
major/minor, manufacturer's name string.
- ^ _after_ the call to `cdev_add()`, probe the PCIe bus to find the
real Kyouto3 device by calling:

`Jan 19`

_(call ofter `cdev_add()`:)_

    pci_register_driver(&kyouto3_pci_drv);

where (`pci.h`):

    struct
