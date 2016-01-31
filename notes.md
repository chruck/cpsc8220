<!-- vim: set filetype=markdown : -->

# CPSC 8220:  Case Study in Operating Systems
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
2. ?
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

## Section 0:
- Warm-up exercise (not part of kernel: get used to system calls)
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

(File descriptor numberss from above diagram:)

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
- module init \_ macros/functions
- module exit /

- `/sbin/insmod mymod.ko`  <- kernel object
- `/sbin/rmmod mymod`  <- no '`.ko`'
- `/sbin/lsmod`

On load: `printk()` from kernel
- `printk()` needs leading qualifier:  priority (0-7: lower is more urgent)
- console priority - messages recorded or not

`Jan 14`
