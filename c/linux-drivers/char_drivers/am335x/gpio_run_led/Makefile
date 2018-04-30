# Makefile2.6  
ifneq ($(KERNELRELEASE),)  
#kbuild syntax. dependency relationshsip of files and target modules are listed here.  
   
mymodule-objs := pri_gpio_driver.o 
obj-m := pri_gpio_driver.o 
     
else  
include ../Rules.mk
PWD  := $(shell pwd)  
all:  
	$(MAKE_CROSS)
#	$(MAKE) -C $(KDIR) M=$(PWD) $(EXTRA_CFLAGSS)
#	$(MAKE) -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=/usr/local/ti-sdk-am335x-evm/linux-devkit/bin/arm-arago-linux-gnueabi- 
clean:  
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.order *.symvers 	     
endif
