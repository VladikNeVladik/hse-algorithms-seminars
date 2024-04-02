#-----------
# Toolchain
#-----------

CC   = x86_64-linux-gnu-gcc
AS   = nasm
QEMU = qemu-system-i386
GDB  = gdb-multiarch

#-----------------------
# Compiler/linker flags
#-----------------------

CFLAGS = \
	-m32 \
	-nostdlib \
	-Wfatal-errors \
	-fno-pie \
	-g

ASFLAGS = \
	-f elf32 \
	-DUNIX \
	-g

LDFLAGS = \
	-m32 \
	-nostdlib \
	-g \
	-Wl,-T,src/helloworld.lds

#-------
# Files
#-------

SOURCES = \
	src/multiboot.asm \
	src/main.asm

OBJECTS_HALFWAY_DONE = $(SOURCES:src/%.c=build/%.o)
OBJECTS              = $(OBJECTS_HALFWAY_DONE:src/%.asm=build/%.o)

EXECUTABLE = build/helloworld

#---------------
# Build scripts
#---------------

all: $(EXECUTABLE) $(SOURCES)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.asm
	@mkdir -p build
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf build

#----------------------
# Emulator interaction
#----------------------

QEMU_FLAGS = \
	-machine pc \
	-cpu base \
	-kernel $(EXECUTABLE) \
	-display none \
	-serial stdio \
	-m 32M \
	-icount 25 \
	-gdb tcp::1234

GDB_FLAGS = \
	--eval-command="set architecture i386" \
	--eval-command="target remote localhost:1234" \
	--eval-command="file $(EXECUTABLE)"

GDB_NATIVE_FLAGS = \
	--eval-command="set architecture i386" \
	--eval-command="file $(EXECUTABLE)" \
	--eval-command="b _start" \
	--eval-command="run"

qemu: $(EXECUTABLE)
	$(QEMU) $(QEMU_FLAGS)

gdb: $(EXECUTABLE)
	$(GDB) $(GDB_FLAGS)

gdb-native: $(EXECUTABLE)
	$(GDB) $(GDB_NATIVE_FLAGS)

.PHONY: all clean qemu gdb
