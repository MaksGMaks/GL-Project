# GL-Project

## How to Build

### Building for OpenWRT on Linux

To build the project for an OpenWRT system, you need to download the OpenWRT SDK for the required architecture.

#### 1. Download the OpenWRT SDK
Visit the official OpenWRT downloads page:

```
https://downloads.openwrt.org/releases/
```

#### 2. Build the SDK
Unpack the downloaded archive and build the SDK using the following command:

```
make defconfig
```

> **Note:** Before building, ensure that GNU awk is installed. If not, install it using a package manager.
> Example for Debian-based systems:
>
> ```
> sudo apt install gawk
> ```

#### 3. Configure Environment Variables
After successfully building the SDK, add the following lines to your `.bashrc` file to use the SDK:

```sh
export STAGING_DIR="/path/to/openwrt/staging_dir"
alias openwrt++="$STAGING_DIR/toolchain-x86_64_gcc-12.3.0_musl/bin/x86_64-openwrt-linux-g++"
```

Replace `/path/to/openwrt` with the actual path to the unpacked OpenWRT SDK directory.

#### 4. Build the Project
Navigate to the project directory and run:

```sh
mkdir build && cd build
openwrt++ ../src/*.cpp -o openwrtnettrace
```

- You can rename `openwrtnettrace` to your preferred name.
- `openwrt++` is the alias you set earlier for the OpenWRT cross-compiler.

---

### Building for Linux

To build the project for a standard Linux system, run the following commands:

```sh
mkdir build && cd build
g++ ../src/*.cpp -o nettrace
```

- You can rename `nettrace` to your preferred name.
- Ensure `g++` is installed on your system (`sudo apt install g++` on Debian-based systems).