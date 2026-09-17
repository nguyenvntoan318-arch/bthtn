CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

TARGET = b1_tuan2

CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -Wall -ffreestanding -fno-builtin
LDFLAGS = -T stm32f103.ld -nostdlib

all: $(TARGET).bin

$(TARGET).elf: main.o startup_stm32f103.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

startup_stm32f103.o: startup_stm32f103.s
	$(CC) $(CFLAGS) -c startup_stm32f103.s -o startup_stm32f103.o

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

clean:
	rm -f *.o *.elf *.bin
