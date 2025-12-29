# Performance Report

This demo shows the engine's performance on a [game written with it](https://github.com/Sibiri4ok/Half-Life-3), at a target resolution of `1600x900` pixels.

**[Watch the performance demo](https://drive.google.com/file/d/1JZhuDTtMXrO5IbhyXu3KMq8SMozNSX9K/view?usp=sharing)**

#### In the video:

1) The required resolution (`1600x900`) being set in the game's code.
2) Gameplay with movement (running through the scene).
3) Real-time FPS (Frames Per Second) counter displayed in the terminal, demonstrating performance.
4) `htop` utility output, showing the overall CPU and memory load of the system during the benchmark.

## Hardware

- **Device:** Apple MacBook Air
- **Chip:** Apple M1
- **RAM:** 8 GB

## Core Optimization Implemented
The achieved performance is a direct result of `Frustum Culling` optimization:

The engine performs visibility calculation for every frame. It renders only the entities and tiles that are currently within the camera's view (frustum). Objects outside the visible screen area are not processed by the render pipeline, significantly reducing the per-frame workload.
