EXTRA_CFLAGSS = -I ../Include 
#KDIR := /usr/src/linux-headers-$(shell uname -r)
KDIR := /usr/local/ti-sdk-am335x-evm/board-support/linux-3.2.0-psp05.06.00.00
#CC = gcc
CC = /usr/local/ti-sdk-am335x-evm/linux-devkit/bin/arm-arago-linux-gnueabi-gcc
#MAKE_CROSS = $(MAKE) -C $(KDIR) M=$(PWD) $(EXTRA_CFLAGSS)
MAKE_CROSS = $(MAKE) -C $(KDIR) M=$(PWD) $(EXTRA_CFLAGSS)   modules ARCH=arm CROSS_COMPILE=/usr/local/ti-sdk-am335x-evm/linux-devkit/bin/arm-arago-linux-gnueabi- 
