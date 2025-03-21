# Pull image with custum minimal cross compiler
FROM rsabbah/x86-elf-gcc

# Update and install dependencies
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y make
RUN apt-get install -y nasm
RUN apt-get install -y xorriso
RUN apt-get install -y mtools
RUN apt-get install -y grub-efi-amd64
RUN apt-get install -y grub-pc-bin

# Set shared volume
VOLUME /root/KFS
WORKDIR /root/KFS
