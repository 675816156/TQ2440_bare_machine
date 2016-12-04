CROSS_COMPILE:=arm-none-linux-gnueabi-
CC:=$(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)ld
OBJCOPY:=$(CROSS_COMPILE)objcopy
OBJDUMP:=$(CROSS_COMPILE)objdump

CFLAGS:=-fno-builtin -nostdinc -nostdlib -g
LDFLAGS:=-Tlink.lds -L/opt/arm-2014.05/lib/gcc/arm-none-linux-gnueabi/4.8.3

TARGET:=spi_flash.bin

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET):main.o start.o uart.o led.o spi_flash.o
	$(LD) $(LDFLAGS) $^ -lgcc -o temp
	$(OBJCOPY) -O binary -S temp $@
	$(OBJDUMP) -D -m arm temp >dis

clean:
	rm -rf *.o $(TARGET) temp dis
