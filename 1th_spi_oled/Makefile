CROSS_COMPILE:=arm-none-linux-gnueabi-
CC:=$(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)ld
OBJCOPY:=$(CROSS_COMPILE)objcopy
OBJDUMP:=$(CROSS_COMPILE)objdump

CFLAGS:=-fno-builtin -nostdinc -nostdlib -g
LDFLAGS:=-nostdlib -Tlink.lds

TARGET:=oled.bin

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET):main.o start.o oled.o
	$(LD) $(LDFLAGS) $^ -o temp
	$(OBJCOPY) -O binary -S temp $@
	$(OBJDUMP) -D -m arm temp >dis

clean:
	rm -rf *.o $(TARGET) temp dis
