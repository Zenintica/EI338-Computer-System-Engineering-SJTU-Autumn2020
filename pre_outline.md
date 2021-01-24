# Project 验收提纲

## Preliminaries

- ssh ubuntu@111.230.168.157
- sudo -i
- switch to root@VM-0-6-ubuntu:/home/ubuntu/CSE

## proj 1

### /proc/jiffies

加载之后，调用cat /proc/jiffies来显示此时变量jiffies的数值。

- dmesg -c *（清除内核消息缓存）*

- make *（编译工程文件）*

- insmod jiffies.ko *（将jiffies.ko加载到内核中）*

- cat /proc/jiffies *（显示此时的jiffies）*

- cat /proc/jiffies *（显示此时的jiffies）*

- rmmod jiffies *（从内核卸载jiffies.ko）*

- dmesg *（显示内核消息）*

### /proc/seconds

加载之后，调用cat /proc/seconds来显示从加载此模块至今度过的时间，单位为秒。

- dmesg -c *（清除内核消息缓存）*

- make *（编译工程文件）*

- insmod seconds.ko *（将seconds.ko加载到内核中）*

- cat /proc/seconds *（显示此时的时间）*

- cat /proc/seconds *（显示此时的时间）*

- rmmod seconds *（从内核卸载jiffies.ko）*

- dmesg *（显示内核消息）*

## proj 2

### proj 2.1: UNIX SHELL

- make

- ./simple-shell

- echo Hello World!

- ls -al

- ls -al > info.txt

- cat < info.txt | sort > info_sorted.txt

- cat info_sorted.txt

- !!

- exit

### proj 2.2: kernel module for task information

- dmesg -c

- netstat -ntlp

- make

- insmod pid.ko

- echo 1 > /proc/pid

- cat /proc/pid

- echo () > /proc/pid *（netstat里面随便找一个pid）*

- cat /proc/pid

- rmmod pid

- dmesg -c

# Proj 3

- make

- ./banker 10 6 7 8 *（初始化）*

- RQ 0 3 1 2 1 *（进程0要求四种资源分别是3 1 2 1）*

- RL 0 1 1 0 1

- RQ 0 2 2 2 2

- RQ 1 2 2 2 2

- RQ 2 2 2 2 2 *(这里请求拒绝，资源不足)*

- RQ 2 2 2 1 2 *(这里请求拒绝，分配后状态不安全)*

- “*”

# Proj 4

## basic operations showing RQ and RL

- RQ P1 10000 F

- RQ P2 20000 F

- RQ P3 30000 F

- RQ P4 30000 F

- STAT

- RL P4

- RQ P4 40000 F

- STAT

- RQ P114 51400 W

## get to a particular state for showcases

- RQ P4 40000 F

- RL P1

- RQ P20 5000 F

- RQ P21 2500 F

- RL P20

- RL P3

- STAT

## 展示F，W和B

- RQ P31 999 F

- STAT

- RL P31

- RQ P32 999 W

- STAT

- RL P32

- RQ P33 999 B

- STAT