# Run this Makefile as follows: 
# $(MAKE) -C $(KDIR) M=$(PWD) modules 

KDIR = /lib/modules/$(shell uname -r)/build 

obj-m := skeleton.o

all : 
	$(MAKE) -C $(KDIR) M=$(PWD) modules 

install: 
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install 
	depmod -a

clean: 
	rm -f *~
	$(MAKE) -C $(KDIR) M=$(PWD) clean

