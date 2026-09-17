CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size
CFLAGS = -mcpu=cortex-m3 -mthumb -std=c11 -Os -g3 -Wall -Wextra -Werror -ffreestanding -ffunction-sections -fdata-sections -fno-unwind-tables -fno-asynchronous-unwind-tables
LDFLAGS = -nostdlib -Wl,--gc-sections -Wl,-z,noexecstack -Wl,-Map=build/bai05.map -T stm32f103c8.ld
OBJECTS = build/main.o build/app.o build/rx.o build/startup.o
.PHONY: all clean flash test
all: build/bai05.bin
build:
	mkdir -p build
build/%.o: %.c app.h rx.h | build
	$(CC) $(CFLAGS) -c $< -o $@
build/startup.o: startup.s | build
	$(CC) -mcpu=cortex-m3 -mthumb -g3 -c $< -o $@
build/bai05.elf: $(OBJECTS) stm32f103c8.ld
	$(CC) -mcpu=cortex-m3 -mthumb $(LDFLAGS) $(OBJECTS) -lgcc -o $@
	$(SIZE) $@
build/bai05.bin: build/bai05.elf
	$(OBJCOPY) -O binary $< $@
flash: build/bai05.bin
	st-flash --reset write build/bai05.bin 0x08000000
test: | build
	gcc -std=c11 -Wall -Wextra -Werror -I. tests/test_logic.c app.c rx.c -o build/test_logic
	./build/test_logic
clean:
	rm -rf build
