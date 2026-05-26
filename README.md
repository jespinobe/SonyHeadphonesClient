# Sony Headphones Client (Enhanced Fork)

This project is a maintained fork of the original Sony Headphones Client by Plutoberth.

The original project was archived, so this fork continues development with fixes, Linux improvements, UI enhancements and better compatibility with modern systems.

<p align="center">
  <h1 align="center">Sony Headphones Client</h1>

A desktop alternative to Sony's mobile-only Headphones app.

Manage Sony headphone settings directly from Linux, Windows and macOS.

Improved fork with a modernized interface and enhanced Linux support.

</p>

---

## What's New In This Fork

### UI Improvements
- Redesigned and cleaner interface
- Full-screen adaptive layout
- Improved spacing and readability
- Better organization of settings sections
- Improved status display and connection information
- Better visual feedback while commands are being sent

### Linux Improvements
- Improved compatibility with Wayland
- Better Bluetooth connection handling
- Fixed listener initialization issues
- Proper background thread startup
- Fixed application hanging on exit
- Better asynchronous handling

### Device Improvements
- Better support for WH-1000XM4
- Automatic state querying after connection
- Capability refresh after connection
- Improved initialization sequence
- More reliable command transmission

### Stability Improvements
- Reduced duplicate command sending
- Settings are only sent when values actually change
- Better error handling
- Improved disconnect handling
- Background processes terminate correctly

---

## Features

- [x] Ambient Sound Control
- [x] Noise Cancelling Control
- [x] Speak To Chat
- [x] Multipoint Device Controls
- [x] Virtual Sound / VPT
- [x] Sound Position
- [x] Optimizer
- [x] Improved Linux Bluetooth support
- [x] WH-1000XM4 improvements
- [ ] Battery display
- [ ] Equalizer
- [ ] Reading all existing device settings

---

## Supported Platforms

- [x] Linux
- [x] Windows
- [x] macOS
- [x] Wayland
- [ ] ~~TempleOS~~

---

## Supported Headsets

Known working devices:

- WH-1000XM3
- WH-1000XM4
- MDR-XB950BT

Other devices may work as well.

Please report compatibility results.

---

## Screenshots

Enhanced Linux UI:
<p align="center">
<img src="static/old-ui.png" width="45%">
<img src="static/newUI.png" width="45%">
</p>

Original UI vs Enhanced Fork

Modernized interface with:
- improved layout
- better Wayland compatibility
- cleaner organization
- enhanced Sony XM4 support

---

## Installation

### Clone repository

Use recursive clone:

```bash
git clone --recurse-submodules https://github.com/jespinobe/SonyHeadphonesClient.git
```

If submodules fail:

```bash
git submodule sync
git submodule update --init --recursive
```

---

# Build Instructions

## Linux

Debian / Ubuntu:

```bash
sudo apt install \
libbluetooth-dev \
libglew-dev \
libglfw3-dev \
libdbus-1-dev
```

Create build directory:

```bash
cd Client
mkdir build
cd build
```

Configure:

```bash
cmake ..
```

If CMake policy errors appear:

```bash
cmake .. -DCMAKE_POLICY_VERSION_MINIMUM=3.5
```

Compile:

```bash
cmake --build . -j$(nproc)
```

Run:

```bash
./SonyHeadphonesClient
```

---

### Fedora

```bash
sudo dnf install \
bluez-libs-devel \
glew-devel \
glfw-devel \
dbus-devel
```

---

## Windows

Install:

- Visual Studio Community 2022
- C++ Desktop Components
- Windows SDK
- CMake

Build:

```bash
cd Client
mkdir build
cd build

cmake ..
cmake --build .
```

---

## macOS

Use the provided Xcode project.

---

## Linux Notes

If Bluetooth connects but controls do not appear:

Verify:

- Headphones are paired
- Headphones are connected through BlueZ
- RFCOMM service is discovered
- Audio profile is active

Useful commands:

Check devices:

```bash
bluetoothctl devices
```

Connect manually:

```bash
bluetoothctl
connect XX:XX:XX:XX:XX
```

Monitor Bluetooth logs:

```bash
journalctl -f | grep bluetooth
```

---

## Development Notes

This fork includes fixes for:

### Listener startup

Background listener now starts correctly:

```cpp
this->_listener =
std::make_unique<Listener>(
    this->_headphones,
    this->_bt
);
```

### Automatic state sync

After connection:

```cpp
this->_headphones.queryState();
```

### Clean shutdown

Background tasks now terminate correctly when application exits.

---

## Original Project

Original archived repository:

https://github.com/Plutoberth/SonyHeadphonesClient

Huge thanks to everyone involved.

---

## Contributors

Original:

* [Plutoberth](https://github.com/Plutoberth) - Initial Work and Windows Version 
* [Mr-M33533K5](https://github.com/Mr-M33533K5) - BT Interface and Other Help 
* [semvis123](https://github.com/semvis123) - macOS Version * [jimzrt](https://github.com/jimzrt) - Linux Version 
* [guilhermealbm](https://github.com/guilhermealbm) - Noise Cancelling Switch

Fork additions:

- jespinobe
- Linux UI redesign
- Wayland support improvements
- Bluetooth fixes
- WH-1000XM4 improvements
- Connection stability improvements

---

## License

Distributed under the MIT License.

See LICENSE for more information.