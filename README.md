# Spin-Stabilized Sounding Rocket Simulator

## 🚀 Project Overview
This project is a physics-based simulation of a sounding rocket developed in C++ from scratch. The goal is to simulate the complete flight profile of a rocket—from lift-off to atmospheric reentry and impact—while implementing a custom physics engine without relying on external math libraries (like GLM or Eigen).

The simulation focuses on **Newtonian Mechanics**, **Aerodynamics**, and **3D Coordinate Transformations** using Quaternions.

## 📚 Theoretical Background
This project is built upon the following engineering concepts derived from MIT 16.07 Dynamics Lecture Notes:

* [cite_start]**Newton's Second Law:** ($F = ma$) governing the rocket's acceleration [cite: 74-75].
* [cite_start]**Aerodynamic Drag:** Calculating the resistive force of the atmosphere using the drag equation and ballistic coefficients [cite: 730-733].
* [cite_start]**Terminal Velocity:** Simulating the equilibrium state where drag equals gravity during descent [cite: 743-745].
* [cite_start]**Rotational Dynamics:** Using Quaternions to handle spin stabilization and coordinate transformations (Local to Global frame)[cite: 511, 540].
* [cite_start]**The Transport Theorem:** Calculating the kinematic properties of a sensor mounted on a rotating body[cite: 293, 304].

## 🛠️ Tech Stack
* **Language:** C++ (Standard Library only)
* **Math:** Custom `Vector3` and `Quaternion` structures.
* [cite_start]**Integration Method:** Explicit Euler Integration[cite: 678].
* **Platform:** Linux (WSL/Ubuntu).

## 📂 Project Structure
* `src/` - Source files (.cpp).
* `include/` - Header files (.h) for Vector3, Quaternion, and Rocket classes.
* `data/` - Output CSV files for telemetry analysis.

## ⚡ How to Build and Run
1.  **Compile:**
    ```bash
    g++ main.cpp -o rocket_sim
    ```
2.  **Run:**
    ```bash
    ./rocket_sim
    ```
3.  **Visualize:**
    The program outputs a `.csv` file. Import this into Excel or Python (Matplotlib) to visualize the Altitude vs. Time and Velocity vs. Time graphs.