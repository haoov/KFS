# Project
TARGET			:= kfs.iso
ARCH			:= i686
DIST_DIR		:= dist
BUILD_DIR		:= build
SRC_DIR			:= src
TARGET_DIR		:= targets

ARCH_SRC		:= $(SRC_DIR)/$(ARCH)
ARCH_BUILD		:= $(BUILD_DIR)/$(ARCH)
ARCH_DIST		:= $(DIST_DIR)/$(ARCH)
ARCH_TARGET		:= $(TARGET_DIR)/$(ARCH)

KERNEL_SRC		:= $(ARCH_SRC)/kernel/src
KERNEL_BUILD	:= $(ARCH_BUILD)/kernel
KERNEL_INC		:= $(ARCH_SRC)/kernel/inc

# Files
LINKER			:= $(ARCH_TARGET)/linker.ld
ASM_SRCS		:= $(shell find $(ARCH_SRC) -name "*.asm")
ASM_OBJS		:= $(patsubst $(ARCH_SRC)/%.asm,$(ARCH_BUILD)/%.o,$(ASM_SRCS))
KERNEL			:= $(ARCH_TARGET)/iso/boot/kernel.bin

KERNEL_SRCS		:= $(shell find $(KERNEL_SRC) -name "*.c")
KERNEL_OBJS		:= $(patsubst $(KERNEL_SRC)/%.c,$(KERNEL_BUILD)/%.o,$(KERNEL_SRCS))

# Compiler
CC				:= $(ARCH)-elf-gcc
CFLAGS			:= -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -o0
LINK			:= $(ARCH)-elf-ld
ASM				:= nasm
ASMFLAGS		:= -f elf32

# Building iso for specified ARCH
 build:
	sudo docker build buildenv -t kfs-buildenv
	sudo docker run --rm -v .:/root/KFS kfs-buildenv make $(TARGET) ARCH=$(ARCH)

# Create the iso file with grub installed
$(TARGET): $(KERNEL)
	@mkdir -p $(ARCH_DIST)
	grub-mkrescue -o $(ARCH_DIST)/$(TARGET) $(ARCH_TARGET)/iso

# Link object files with linker.ld as script to create the kernel binary
$(KERNEL): $(ASM_OBJS) $(KERNEL_OBJS)
	$(LINK) -n -o $@ -T $(LINKER) $(ASM_OBJS) $(KERNEL_OBJS) $(KERNEL_LIB)

# Assemble each asm file
$(ARCH_BUILD)/%.o: $(ARCH_SRC)/%.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

# Compile kernel c files
$(KERNEL_BUILD)/%.o: $(KERNEL_SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(KERNEL_INC) -I $(KERNEL_LIB_DIR)/inc

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(KERNEL)
	rm -rf $(KERNEL_LIB)

fclean: clean
	rm -rf $(DIST_DIR)

.PHONY: build all clean fclean
