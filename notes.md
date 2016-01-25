CPSC 8220:  Case Study in Operating Systems
CPSC 8221:  Case Study in Operating Systems Lab

- Jan 7, 2016

Robert Geist
Aubrey Lawson - lab TA

TTh 15:45 - 16:35 - class
W 17:00 - 18:50 - lab: 110D

Content:  how a real operating system works -> Linux 4.3.3

No documentation except source code

Book - Linux Device Drivers 4th ed -> 3.2 kernel -> coming out

_Reasons to take the course_
1) job @ systems software
2) understand system performance

_NOT_
1) satisfy PhD requirement
2) boost GPA

_Text_ (class notes & handouts)
        R. Love: Linux Kernel Development 3rd ed. (2.6 kernel)

_Grades_:
3 Programming Projects -> Teams 15% each
        1) write device driver for graphics card -> IRQs, memory management

Midterm exam - March 8 - 20%
Final exam - Apr 29 - 25%
        both open book/notes/laptop
Lab - 10%

Scale:
85-100 A
70-84  B
55-69  C
54-0   F

_Office Hours_
308 McAdams, 15:00-17:00 MWF (but come anytime)

_Attendance_
"reservation system"
- not required -> no roll
- all tests, projects, hints, etc are class notes
- no individual lectures for missed material
- no handouts

_Prereq_
1) OS Principles (lock hierarchy, race conditions, semaphores)
2) ability to read/write non-trivial C code

Running in VM - KVM
Graphics Card - qemu
QEMU - generic hardware
     - modified for real-looking like real AMD

Section 0:  Warm-up exercise (not part of kernel: get used to system calls)
(handout)
- user-level C code to review
        - provides serial line control
        - actually used to low-level config NetApp filers
- to observe
1) level of C sophistication
        minimum -> starting point
2) examples of important system calls
        - fork()
        - wait()
        - ioctl() - going to implement a lot of these

_BASIC DESIGN_

                     +--------------------+
                     |                    |
  +---------+        |-+ <- (child) <-- +-|        +-----------+
  |         |        | |                | |        |           |
  | monitor |--------| |                | |--------| NetApp    |
  |         |    /-> | |                | | <-\    | (FAS960c) |
  +---------+   /    |-+ -> (parent) -> +-|    \   |           |
 / keyboard /  /     |                    |     \  +-----------+
/----------/  /      +--------------------+      \
            control port       ^          external port (serial line)
                               |
                       launch 2 processes:

[] show me - Geist wrote to write over code in class
           - email request code for
           - draws on top of ASCII text files
           - needs touch-sensitive display

--> Don't have time for error checking in this course, minimal error
checking in our code is sufficient