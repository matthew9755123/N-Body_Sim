# 🌌 N-Body Simulator

A high-performance, real-time gravitational simulator built in C++ using the Barnes-Hut algorithm. This project models and visualizes the interactions between thousands of bodies in 2D space, using multithreading to enable smooth, scalable computation and rendering.

---

## ✨ Features

* **Real-Time Simulation:** Simulates the gravitational interactions of 22,000+ bodies before performance drops.
* **Barnes-Hut Optimization:** Reduces computational complexity using a hierarchical spatial partitioning strategy.
* **Multithreaded Performance:** Uses thread pooling and futures to parallelize force calculations.
* **2D Visualization:** Real-time rendering of the simulation using SFML.

---

## 🚀 Technologies

* **C++** - Core language for performance and simulation logic  
* **Barnes-Hut Algorithm** - For efficient force approximation  
* **BS::thread_pool** - Lightweight C++ thread pool for parallelism  
* **SFML** - Simple and Fast Multimedia Library for 2D graphics and window management

---

## 📸 Screenshots

![simulation example](https://github.com/matthew9755123/N-Body_Sim/blob/main/BHSIM.png)  
*Image: Barnes-Hut Simulation.*

![simulation example](https://github.com/matthew9755123/N-Body_Sim/blob/main/NBODY_SIM.png)  
*Image: Real-time gravitational simulation with multithreaded force calculations.*

---

## 🧠 How It Works

* **Build Quadtree:** Divide the simulation space into quadrants recursively until each contains 1 or 0 bodies.
* **Force Calculation:** Approximate far-away clusters as single mass centers when calculating gravitational pull.
* **Update Step:** Calculate forces in parallel → update velocities → move bodies.
* **Visualization:** Positions are rendered using SFML in a simple 2D window.

---

### Technical Improvements

* **SIMD/Cache Optimization:** Improve memory and CPU efficiency.
* **OpenMP or TBB Integration:** Boost parallel performance using industry-standard libraries.
* **Configurable Simulation Settings:** Add user controls for gravity constant, θ threshold, timestep, etc.

