# obj-m += usb_detect.o

# KDIR=/usr/src/linux-headers-6.2.13-surface

# CFLAGS = -D__KERNEL__ -DMODULE -I$(KDIR)/include -Wall

# all:
# 	$(MAKE) -C $(KDIR) M=$(shell pwd) $(KCONFIG) modules

# clean:
# 	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
# 	rm -f *.o

obj-m += usb_key_driver.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
ARCH := x86_64

all:
	make -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) clean
