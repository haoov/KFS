FROM debian:bookworm

# env
ENV BINUTILS_V=2.44
ENV GCC_V=14.2.0

# Update and upgrade packages
RUN apt update -y
RUN apt upgrade -y

# Install dependencies
RUN apt install wget -y
RUN apt install build-essential -y
RUN apt install bison -y
RUN apt install flex -y
RUN apt install libgmp3-dev -y
RUN apt install libmpc-dev -y
RUN apt install libmpfr-dev -y
RUN apt install texinfo -y
RUN apt install libisl-dev -y

RUN mkdir /root/src
WORKDIR /root/src

# Set env
ENV PREFIX=/root/opt/cross
ENV TARGET=x86_64-elf
ENV PATH="$PREFIX/bin:$PATH"

# Download binutils
RUN	wget "https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_V.tar.gz" && \
	tar -xf binutils-$BINUTILS_V.tar.gz && \
	rm -rf binutils-$BINUTILS_V.tar.gz

# Download gcc
RUN	wget "https://ftp.gnu.org/gnu/gcc/gcc-$GCC_V/gcc-$GCC_V.tar.gz" && \
	tar -xf gcc-$GCC_V.tar.gz && \
	rm -rf gcc-$GCC_V.tar.gz

# Build binutils
RUN	mkdir build-binutils && \
	cd build-binutils && \
	../binutils-$BINUTILS_V/configure --target=$TARGET \
										--prefix="$PREFIX" \
										--with-sysroot \
										--disable-nls \
										--disable-werror && \
	make && \
	make install

#Build gcc
RUN	mkdir build-gcc && \
	cd build-gcc && \
	../gcc-${GCC_V}/configure	--target=${TARGET} \
								--prefix="${PREFIX}" \
								--disable-nls \
								--enable-languages=c \
								--without-headers \
								--disable-hosted-libstdcxx && \
	make all-gcc && \
	make all-target-libgcc && \
	make install-gcc && \
	make install-target-libgcc
