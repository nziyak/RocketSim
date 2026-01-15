# Development Roadmap

## Phase 1: The Mathematical Foundation (No External Libraries)
*Objective: Build the tools required to calculate physics in 3D space.*

- [d] **Implement `Vector3` Class**
    - [d] Store `x`, `y`, `z` (floats).
    - [d] Implement Operator Overloading (`+`, `-`, `*` scalar).
    - [d] Implement `DotProduct`.
    - [d] [cite_start]Implement `CrossProduct`.
    - [d] Implement `Magnitude` and `Normalize` functions.

- [d] **Implement `Quaternion` Class**
    - [d] Store `x`, `y`, `z`, `w`.
    - [d] Implement `AngleAxis(angle, axis)` static function for creating rotations.
    - [d] Implement `RotateVector(vector)`: Logic to rotate a vector using the quaternion.
    - [d] Implement Operator Overloading (`*`) for combining rotations.

## Phase 2: The Physics Engine Core
*Objective: Create the Rocket object and define its physical properties.*

- [d] **Create `Rocket` Class**
    - [d] Define State Variables: `position`, `velocity`, `orientation` (Quaternion).
    - [d] Define Constants:
        - [d] Mass ($m = 50.0$ kg)
        - [d] Area ($A = 0.1$ $m^2$)
        - [d] Drag Coefficient ($C_d = 0.5$)
        - [d] Thrust ($F_{thrust} = 2000.0$ N)
    - [d] Define `Update(float dt)` method signature.

## Phase 3: Simulation Logic (The Update Loop)
*Objective: Implement the physics math inside the `Update` function.*

- [d] **Step 1: Coordinate Transformation (Local -> Global)**
    - [d] Calculate `ThrustVector`.
    - [d] Logic: Rotate the local "Up" vector `(0, 1, 0)` by the rocket's current `orientation` quaternion.

- [d] **Step 2: Force Calculation**
    - [d] [cite_start]Calculate Gravity Vector ($F_g = m \cdot g$ downward).
    - [d] Calculate Aerodynamic Drag Vector ($F_{drag}$):
        - [d] [cite_start]Formula: $-\frac{1}{2} \cdot \rho \cdot v^2 \cdot C_d \cdot A$.
        - [d] Ensure drag acts in the opposite direction of velocity.

- [d] **Step 3: Newton's Second Law**
    - [d] [cite_start]Sum all forces: $F_{net} = F_{thrust} + F_{gravity} + F_{drag}$[cite: 74].
    - [d] Calculate Acceleration: $a = F_{net} / m$.

- [d] **Step 4: Numerical Integration (Euler)**
    - [d] Update Velocity: $v_{new} = v_{old} + a \cdot dt$.
    - [d] Update Position: $p_{new} = p_{old} + v_{new} \cdot dt$.

- [d] **Step 5: Spin Stabilization**
    - [d] Define turn speed (e.g., 360 degrees/sec).
    - [d] Create a "spin" quaternion representing the incremental rotation for this frame.
    - [d] Update `orientation`: Multiply current orientation by the spin quaternion.

## Phase 4: Mission Control (Main Loop)
*Objective: Manage the flight stages and data recording.*

- [ ] **Implement `main` Loop**
    - [ ] Initialize Rocket at $(0, 0, 0)$.
    - [ ] Create a simulation loop with a fixed time step ($dt = 0.01s$).

- [ ] **Implement Flight Logic**
    - [ ] **Lift-off:** Engine ON.
    - [ ] **Burnout:** Check if $t > 10s$ (or fuel empty) $\rightarrow$ Set Thrust to 0.
    - [ ] **Apogee:** Observe where velocity crosses 0.
    - [ ] **Re-entry:** Watch drag forces increase.
    - [ ] **Impact:** Stop simulation when `position.y < 0`.

- [ ] **Data Logging**
    - [ ] Output `Time`, `Altitude`, `Velocity`, and `Acceleration` to a `.csv` file.

## Phase 5: Bonus - The Sensor Problem
*Objective: Apply the Transport Theorem for a wing-tip sensor.*

- [ ] **Implement Transport Theorem Logic**
    - [ ] Define Sensor Offset $r$ (e.g., 1 meter on X-axis).
    - [ ] Calculate Angular Velocity Vector $\Omega$.
    - [ ] [cite_start]Compute Sensor Velocity: $v_{sensor} = v_{rocket} + (\Omega \times r_{global})$[cite: 293].
    - [ ] Log sensor velocity to verify the "Coriolis/Rotational" effect.