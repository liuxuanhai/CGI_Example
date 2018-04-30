# Makefile2.6
ifneq ($(KERNELRELEASE),)
#kbuild syntax. dependency relationshsip of files and target modules are listed here.

mymodule-objs := second_dev.o
obj-m := second_dev.o

else
PWD  := $(shell pwd)

KVER ?= $(shell uname -r)
#KDIR := /lib/modules/$(KVER)/build #KDIR目录其实是链接到上面那个Makefile中的那个  /usr/src/linux-source-2.6.15/*中
KDIR := /usr/src/linux-headers-$(shell uname -r)
all:
	$(MAKE) -C $(KDIR) M=$(PWD)
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.order *.symvers
endif
