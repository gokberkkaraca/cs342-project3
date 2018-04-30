obj-m += mymodule.o

all: app
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

app: app.c
	gcc -Wall -g -o app app.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
