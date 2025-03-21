# Project
TARGET			:= kfs.iso
ARCH			?= i686
DIST_DIR		:= dist
BUILD_DIR		:= build
SRC_DIR			:= src
TARGET_DIR		:= targets

ARCH_SRC		:= $(SRC_DIR)/$(ARCH)
ARCH_BUILD		:= $(BUILD_DIR)/$(ARCH)
ARCH_DIST		:= $(DIST_DIR)/$(ARCH)
ARCH_TARGET		:= $(TARGET_DIR)/$(ARCH)

KERNEL_SRC		:= $(SRC_DIR)/kernel/src
KERNEL_BUILD	:= $(BUILD_DIR)/kernel
KERNEL_INC		:= $(SRC_DIR)/kernel/inc

# Files
ARCH_LINKER		:= $(ARCH_TARGET)/linker.ld
ARCH_ASM_SRCS	:= $(wildcard $(ARCH_SRC)/boot/*.asm)
ARCH_ASM_OBJS	:= $(patsubst $(ARCH_SRC)/boot/%.asm,$(ARCH_BUILD)/%.o,$(ARCH_ASM_SRCS))
ARCH_KERNEL		:= $(ARCH_TARGET)/iso/boot/kernel.bin

KERNEL_SRCS		:= $(wildcard $(KERNEL_SRC)/*.c)
KERNEL_BUILDS	:= $(patsubst $(KERNEL_SRC)/%.c,$(KERNEL_BUILD)/%.o,$(KERNEL_SRCS))

# Compiler
CC				:= $(ARCH)-elf-gcc
CFLAGS			:= -ffreestanding
LINK			:= $(ARCH)-elf-ld
ASM				:= nasm
ASMFLAGS		:= -f elf32
ifeq ($(ARCH), x86_64)
	ASMFLAGS	:= -f elf64
endif

# Building iso for specified ARCH
 build:
	sudo docker build buildenv -t kfs-buildenv
	sudo docker run --rm -v .:/root/KFS kfs-buildenv make $(TARGET) ARCH=$(ARCH)

# Create the iso file with grub installed
$(TARGET): $(ARCH_KERNEL) | $(ARCH_DIST)
	grub-mkrescue -o $(ARCH_DIST)/$(TARGET) $(ARCH_TARGET)/iso

# Link object files with linker.ld as script to create the kernel binary
$(ARCH_KERNEL): $(ARCH_ASM_OBJS) $(KERNEL_BUILDS)
	$(LINK) -n -o $@ -T $(ARCH_LINKER) $(ARCH_ASM_OBJS) $(KERNEL_BUILDS)

# Assemble each asm file
$(ARCH_BUILD)/%.o: $(ARCH_SRC)/boot/%.asm | $(ARCH_BUILD)
	$(ASM) $(ASMFLAGS) $< -o $@

# Compile kernel c files
$(KERNEL_BUILD)/%.o: $(KERNEL_SRC)/%.c | $(KERNEL_BUILD)
	$(CC) -c $< -o $@ -I $(KERNEL_INC) -ffreestanding

# Create directory
$(ARCH_BUILD) $(ARCH_DIST) $(KERNEL_BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(ARCH_KERNEL)

fclean: clean
	rm -rf $(DIST_DIR)

.PHONY: build all clean fclean
