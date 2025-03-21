# GL-Project

## How to build
### For OpenWRT from Linux
To build project for OpenWRT system, you need to download OpenWRT-SDK for required machine.
Website for download: 
``
https://downloads.openwrt.org/releases/
``
Next step is building SDK. Unpack archive and build SDK using:
``
make defconfig
``
Note: before building make sure GNU awk is installed. If not, use any package installer. 
Here is example for Ubuntu/Debian like systems:
``
sudo apt install gawk
``
After successfull building add next lines into .bashrc file to use sdk:
``
export STAGING_DIR='PATH_TO_OPENWRT_DIR/staging_dir'
alias openwrt++="$STAGING_DIR/toolchain-x86_64_gcc-12.3.0_musl/bin/x86_64-openwrt-linux-g++"
``
PATH_TO_OPENWRT_DIR is unpacked directory. You can use another name for allias.

Finally, go into this project dir and run:
``
mkdir build && cd build
openwrt++ ./src/*.cpp -o ./build/openwrtnettrace
``
"openwrtnettrace" can be renamed as you wish. "openwrt++" is name of your alias.

### For Linux
To build project for Linux system, use:
``
mkdir build && cd build
g++ ./src/*.cpp -o ./build/nettrace
``
"nettrace" can be renamed as you wish.