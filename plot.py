import pandas as pd
import matplotlib.pyplot as plt

# CSV'yi oku
data = pd.read_csv("flight_data.csv")

time = data["time"]
altitude = data["altitude"]
velocity = data["velocity"]
acceleration = data["acceleration"]

# --- Altitude ---
plt.figure()
plt.plot(time, altitude)
plt.xlabel("Time (s)")
plt.ylabel("Altitude (m)")
plt.title("Rocket Altitude vs Time")
plt.grid(True)

# --- Velocity ---
plt.figure()
plt.plot(time, velocity)
plt.xlabel("Time (s)")
plt.ylabel("Velocity (m/s)")
plt.title("Rocket Velocity vs Time")
plt.grid(True)

# --- Acceleration ---
plt.figure()
plt.plot(time, acceleration)
plt.xlabel("Time (s)")
plt.ylabel("Acceleration (m/s^2)")
plt.title("Rocket Acceleration vs Time")
plt.grid(True)

plt.show()
