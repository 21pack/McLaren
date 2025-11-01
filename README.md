# McLaren

Modern C++ Library for Axonometric Rendering Engine

## Overview

McLaren is a C++ game engine specializing in isometric rendering. Built on top of [SFML](https://www.sfml-dev.org), it provides window management, input handling and 2D rendering using only point drawing.

### Features

* **Cross-Platform:** Runs on Windows, Linux, and macOS.
* **Isometric Rendering:** True isometric projection with depth sorting.
* **Sprites:** Animated objects and any sprite sheets.
* **Dynamic Shadows.**
* **Entity Component System:** Convenient game object management with [entt](https://github.com/skypjack/entt?ysclid=mhfjtbqbpf481919909) library.
* **Performance Optimized:**
  * running the render in a separate thread;
  * initialization of static objects before the rendering loop;
  * view culling for entity.

## Demo Game

**Wolf Meadow** is a sample game demonstrating McLaren's capabilities.It utilizes isometric sprites from [Pixel Isometric Tiles](https://scrabling.itch.io/pixel-isometric-tiles) pack. In this interactive simulation, wolves roam dynamically across a meadow. The demo shows how you can move around the main character, the wolf. How another wolf follows him, and how two other wolves move independently on the map. The demo also shows the quality of the image, navigates around specified obstacles (boulders, water) and the displays shadows and their overlapping.

![demo](/demo.gif)

## API Overview

McLaren provides a clean, intuitive API for game development:

* Creating a scene through the iLoop interface.
* Adding your own components.
* Support for animations, input/output, shadow settings, etc. through ready-made components.

For more detailed information, you need to study the engine code and its documentation.

## Usage

### Install

If you use Linux, install SFML's dependencies using your system package manager. On Ubuntu and other Debian-based distributions you can use the following commands:

```shell
sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libfreetype-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libfreetype-dev
```

### Build

```shell
./scripts/build.sh --release
```
If you want to build without tests:
```shell
./scripts/build.sh --release --without-tests
```
### Play

```shell
./scripts/play.sh
```

### Test

```shell
./scripts/test.sh
```

## Authors

* **Maxim Rodionov:** [GitHub](https://github.com/RodionovMaxim05), [Telegram](https://t.me/Maxoon22)
* **Dmitri Chirkov:** [GitHub](https://github.com/kinokotakenoko9), [Telegram](https://t.me/chdmitri)
* **Nikita Shchutskii:** [GitHub](https://github.com/ns-58), [Telegram](https://t.me/szcz00)
* **Vladimir Zaikin:** [GitHub](https://github.com/Friend-zva), [Telegram](https://t.me/vo_va_w)

## Contributing

Please read our [Contributing Guide](CONTRIBUTING.md) for details on our code style, commit message conventions, and pull request process.

## License

Distributed under the [MIT License](https://choosealicense.com/licenses/mit/). See [`LICENSE`](LICENSE) for more information.
