# Development Roadmap

## Phase 1: The Mathematical Foundation (No External Libraries)
*Objective: Build the tools required to calculate physics in 3D space.*

- [ ] **Implement `Vector3` Struct**
    - [ ] Store `x`, `y`, `z` (floats).
    - [ ] Implement Operator Overloading (`+`, `-`, `*` scalar).
    - [ ] Implement `DotProduct`.
    - [ ] [cite_start]Implement `CrossProduct`[cite: 225, 232].
    - [ ] Implement `Magnitude` and `Normalize` functions.

- [ ] **Implement `Quaternion` Struct**
    - [ ] Store `x`, `y`, `z`, `w`.
    - [ ] Implement `AngleAxis(angle, axis)` static function for creating rotations.
    - [ ] Implement `RotateVector(vector)`: Logic to rotate a vector using the quaternion.
    - [ ] Implement Operator Overloading (`*`) for combining rotations.

## Phase 2: The Physics Engine Core
*Objective: Create the Rocket object and define its physical properties.*

- [ ] **Create `Rocket` Class**
    - [ ] Define State Variables: `position`, `velocity`, `orientation` (Quaternion).
    - [ ] Define Constants:
        - [ ] Mass ($m = 50.0$ kg)
        - [ ] Area ($A = 0.1$ $m^2$)
        - [ ] Drag Coefficient ($C_d = 0.5$)
        - [ ] Thrust ($F_{thrust} = 2000.0$ N)
    - [ ] Define `Update(float dt)` method signature.

## Phase 3: Simulation Logic (The Update Loop)
*Objective: Implement the physics math inside the `Update` function.*

- [ ] **Step 1: Coordinate Transformation (Local -> Global)**
    - [ ] Calculate `ThrustVector`.
    - [ ] Logic: Rotate the local "Up" vector `(0, 1, 0)` by the rocket's current `orientation` quaternion.

- [ ] **Step 2: Force Calculation**
    - [ ] [cite_start]Calculate Gravity Vector ($F_g = m \cdot g$ downward)[cite: 154].
    - [ ] Calculate Aerodynamic Drag Vector ($F_{drag}$):
        - [ ] [cite_start]Formula: $-\frac{1}{2} \cdot \rho \cdot v^2 \cdot C_d \cdot A$[cite: 730].
        - [ ] Ensure drag acts in the opposite direction of velocity.

- [ ] **Step 3: Newton's Second Law**
    - [ ] [cite_start]Sum all forces: $F_{net} = F_{thrust} + F_{gravity} + F_{drag}$[cite: 74].
    - [ ] Calculate Acceleration: $a = F_{net} / m$.

- [ ] **Step 4: Numerical Integration (Euler)**
    - [ ] Update Velocity: $v_{new} = v_{old} + a \cdot dt$.
    - [ ] Update Position: $p_{new} = p_{old} + v_{new} \cdot dt$.

- [ ] **Step 5: Spin Stabilization**
    - [ ] Define turn speed (e.g., 360 degrees/sec).
    - [ ] Create a "spin" quaternion representing the incremental rotation for this frame.
    - [ ] Update `orientation`: Multiply current orientation by the spin quaternion.

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