# SISPS-PV

UCT SISPS Photovoltaic Board Firmware

![latest tag](https://img.shields.io/github/v/tag/lawrencestanton/SISPS-PV.svg?color=brightgreen)
[![CMake](https://github.com/LawrenceStanton/SISPS-PV/actions/workflows/cmake.yml/badge.svg)](https://github.com/LawrenceStanton/SISPS-PV/actions/workflows/cmake.yml)
[![Pages](https://github.com/LawrenceStanton/SISPS-PV/actions/workflows/pages/pages-build-deployment/badge.svg)](https://github.com/LawrenceStanton/SISPS-PV/actions/workflows/pages/pages-build-deployment)

## Overview

The UCT Sea Ice DC System Power Supply (SISPS) project is an embedded, rack-mounted, 12VDC power supply designed to power small to medium power applications (50-100W) in Antarctica. It consists, in part, of Photovoltaic input power (the subject of this repository), a lead acid battery bank, and many other supporting power and embedded electronics.

![Functional Block Diagram](Assets/Images/Subsystem%20Functional%20Diagram.png "SISPS Functional Diagram")

The SISPS-PV board is a custom designed PCB that interfaces with the SISPS main backplane and provides the following functionality:

* Maximum Power Point Tracking (MPPT) for a 50V/50W solar panel.
* Embedded STM32G07 Microcontroller controlling:
  * SM72445 MPPT Controller & SM72295 Full Bridge Driver
  * HDC1080 Temperature & Humidity Sensor
  * 4x TMP116 Temperature Sensors
  * RS485 Interface to Backplane
  * Opto-Isolated UART Interface and Console to User via USB-C
  * Embedded EEPROM for Configuration Storage and Logging
* START and STOP backplane control lines.
* 3V3 VCC Supervision
* 3x GPIO LEDs

## Repository Structure

This repository contains a non trivial file structure and build system. This section will outline the folder structure and build process.

### Folders

#### [Modules](Modules)

Git submodules.

Various submodules are included for first and third party libraries and drivers. These are included as submodules to allow for easy version control and updating. This also allows reuse of first-party drivers. Typically, for each embedded peripheral, there is a submodule written to match.

The following submodules are currently included:

* **Third Party**
  * [**STM32CubeG0**](https://github.com/STMicroelectronics/STM32CubeG0) — ST Microelectronics Support for STM32G0 Devices.
  * [**etl**](https://github.com/ETLCPP/etl) — Embedded Template Library
* **First Party**
  * [**HDC1080**](https://github.com/LawrenceStanton/HDC1080) — Driver for HDC1080 peripheral.
  * [**TMP116**](https://github.com/LawrenceStanton/TMP116) — Driver for TMP116 peripheral.
  * [**SISPS-Commons**](https://github.com/LawrenceStanton/SISPS-Commons) — Common project assets.

> When cloning this repository, submodules must be updated with the following command before they are downloaded to the local clone:
>
> ```zsh
> git submodule update --init --recursive
> ```

First party modules are commonly built with a simple `add_subdirectory()` CMake command, and the libraries provided are linked into the main build in parent `CMakeLists.txt` files.

Third party modules may provide CMake support (like etl), but some repositories (like STM32CubeG0) must be described and built by internal CMake code.

### [Lib](Lib)

Library code for main application.

This folder contains code specific to this application, and is not intended to be reused. This code is built as a static library and linked into the main application. This code is typically written to interface with drivers and provide other general purpose constructs.

Individual libraries are built and then linked into a main library SISPS::Lib. This library is then linked into the main application. Individual libraries may be used instead, typically for testing purposes.

### [Inc](Inc) & [Src](Src)

Main application Include and Source directories.

### [Test](Test)

Functional tests for individual peripherals.

These tests are built as executables and run on the embedded MCU. Each test is focused on a single peripheral and is designed to test the functionality of that peripheral in isolation. The output of these tests is primarily the LED GPIOs on the board, and thus these are semi-manual tests.

Current tests are provided for the following peripherals:

1. [HDC1080](Test/HDC1080/Src/main.cpp)
2. [TMP116](Test/TMP116/Src/main.cpp)

More info on each test may be found in the main.cpp file of each test.

## Build Process

This project uses CMake to configure the build process.

There are several build configurations to choose from, each with a different purpose, which are contained in the [CMakePresets.json](CMakePresets.json) file. These presets are used to configure the build process, and are selected by the user when configuring the build. Most IDEs will recognise this file and integrate with it.

There are 2 major types of build configurations that result in substantially different outputs:

* Crosscompile Builds
* Native Builds

### Crosscompile Builds

Crosscompile builds are intended to produce binaries for the embedded target. These builds are configured to use specifically the ARM GCC Embedded (`arm-none-eabi-`) toolchain. The build artefacts are `.elf` and `.hex` files, which are intended to be flashed to the target, for the main application and functional test applications.

> The build target can be set in your IDE to speed up build times.

These build are set to be placed under `Build/App/`.

### Native Builds

Native build are intended for test binaries to be run on the host machine. This predominantly includes unit tests and test code coverage, which are unsuitable for the embedded target. The build toolchain will default to the CMake defaults for the local system.

> Native builds on Windows fail at runtime due to a suspected compiler issue. This is an unresolved issue, if you must build this project on Windows, please consider resolving this issue and submitting a PR.
>
> [Github Issue](https://github.com/LawrenceStanton/SISPS-PV/issues/1)

Outputs for native builds are placed under `Build/Test/...`.

### User Presets

[CMakePresets.json](CMakePresets.json) is intended for general-purpose use. To override these presets, create a file called `CMakeUserPresets.json` in the root of the repository. This file will be ignored by git. Use this to experiment changes and, if you wish, submit a PR to update the main presets file.

More info on `CMakeUserPresets.json` can be found [here](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html#user-presets-file).

## Continuous Integration

This project uses Github Actions to run CI tests on each commit. These tests are run on on Linux, MacOS, and Windows, for each build configuration.

Branch protection rules are currently not implemented, and so failing commits will still be pushed into the main branch. Implementing this is marked as future work, to be done if and when required for collaboration.

> The accepted solution to a failed commit is to overwrite it with --force.
>
> ```zsh
> git push --force
> ```

## Hardware

The hardware for this project is designed in Altium Designer. The project files are not included in this repository. Due to the limitations of Altium365, the live design files are not available publicly, but can be found in the read-only mirror repository [SISPS-PV-PCB]([https://git](https://github.com/LawrenceStanton/SISPS-PV-PCB)).

More information on the hardware design, including requesting access, can be found in [SISPS-PV-PCB](https://github.com/LawrenceStanton/SISPS-PV-PCB).

Visit the [Github Pages](https://lawrencestanton.github.io/SISPS-PV/) site for this repository to view the latest hardware release.
