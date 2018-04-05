NAME    	= module_keylogger
DIR_TEST	= test/
EXTRA_CFLAGS	= -I$(src)/include/

ifneq ($(KERNELRELEASE),)
	obj-m		+= $(NAME).o
else

	KERNELDIR	?= /lib/modules/$(shell uname -r)/build
	PWD		= $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

endif

clean:
	@rm -f Module.symvers $(NAME).mod.c $(NAME).mod.o $(NAME).o modules.order
	@rm -f built-in.o

fclean: clean
	@rm -f $(NAME).ko

re: clean default

test-build:
	@gcc -o rw_id $(DIR_TEST)rw_id.c
	@gcc -o read_jiffies $(DIR_TEST)read_jiffies.c
	@gcc -o write_foo $(DIR_TEST)write_foo.c
	@gcc -o read_foo $(DIR_TEST)read_foo.c

test-clean:
	@rm -f rw_id
	@rm -f read_jiffies
	@rm -f read_foo
	@rm -f write_foo
