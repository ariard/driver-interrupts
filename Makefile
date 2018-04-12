NAME    	= module_keylogger
DIR_TEST	= test/
EXTRA_CFLAGS	= -I$(src)/include/
SRCS		= srcs/

ifneq ($(KERNELRELEASE),)
	obj-m			+= $(SRCS)/$(NAME).o
	module_keylogger-y	:= $(SRCS)fops.o $(SRCS)state_machine.o $(SRCS)tools.c
else

	KERNELDIR		?= /lib/modules/$(shell uname -r)/build
	PWD			= $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

endif

clean:
	@rm -f Module.symvers $(SRCS)$(NAME).mod.c $(SRCS)$(NAME).mod.o $(SRCS)$(NAME).o modules.order
	@rm -f built-in.o

fclean: clean
	@rm -f $(SRCS)$(NAME).ko

re: clean default
