# Project
TARGET			:= kfs.iso
DIST_DIR		:= dist
BUILD_DIR		:= build
SRC_DIR			:= src
TARGET_DIR		:= targets
X86_64_SRC		:= $(SRC_DIR)/x86_64
X86_64_BUILD	:= $(BUILD_DIR)/x86_64
X86_64_DIST		:= $(DIST_DIR)/x86_64
X86_64_TARGET	:= $(TARGET_DIR)/x86_64

# Files
X86_64_LINKER	:= $(X86_64_TARGET)/linker.ld
X86_64_ASM_SRCS	:= $(wildcard $(X86_64_SRC)/boot/*.asm)
X86_64_ASM_OBJS	:= $(patsubst $(X86_64_SRC)/boot/%.asm,$(X86_64_BUILD)/%.o,$(X86_64_ASM_SRCS))
X86_64_KERNEL	:= $(X86_64_TARGET)/iso/boot/kernel.bin
X86_64_C_SRCS	:= $(wildcard $(SRC_DIR)/kernel/src/*.c)
X86_64_C_OBJS	:= $(patsubst $(SRC_DIR)/kernel/src/%.c,$(BUILD_DIR)/kernel/%.o,$(X86_64_C_SRCS))

# Compiler
X86_64_CC		:= x86_64-elf-gcc
X86_64_LNK		:= x86_64-elf-ld
ASM				:= nasm

# Building iso for x86_64
 build-x86_64:
	sudo docker build buildenv -t kfs-buildenv
	sudo docker run --rm -v .:/root/KFS kfs-buildenv make $(TARGET)

# Create the iso file with grub installed
$(TARGET): $(X86_64_KERNEL) | $(X86_64_DIST)
	grub-mkrescue -o $(X86_64_DIST)/$(TARGET) $(X86_64_TARGET)/iso

# Link object files with linker.ld as script to create the kernel binary
$(X86_64_KERNEL): $(X86_64_ASM_OBJS) $(X86_64_C_OBJS)
	$(X86_64_LNK) -n -o $@ -T $(X86_64_LINKER) $(X86_64_ASM_OBJS) $(X86_64_C_OBJS)

# Assemble each asm file
$(X86_64_BUILD)/%.o: $(X86_64_SRC)/boot/%.asm | $(X86_64_BUILD)
	$(ASM) -f elf64 $< -o $@

# Compile kernel c files
$(BUILD_DIR)/kernel/%.o: $(SRC_DIR)/kernel/src/%.c | $(BUILD_DIR)/kernel
	$(X86_64_CC) -c $< -o $@ -I $(SRC_DIR)/kernel/inc -ffreestanding

# Create directory
$(X86_64_BUILD) $(X86_64_DIST) $(BUILD_DIR)/kernel:
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(X86_64_KERNEL)

fclean: clean
	rm -rf $(DIST_DIR)

.PHONY: build-x86_64 all clean fclean
