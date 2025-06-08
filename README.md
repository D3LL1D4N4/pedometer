# Embedded Step Counter with STM32 and MPU6050

This project implements a simple, real-time step counter (pedometer) using an STM32F401RE Nucleo board and an MPU6050 6-axis IMU sensor. The device reads acceleration data over I²C and processes it using a magnitude-based threshold algorithm to detect steps.

## 🛠️ Hardware

- **Microcontroller**: STM32F401RE (ARM Cortex-M4)
- **Sensor**: MPU6050 (accelerometer used)
- **Communication**:
  - I²C (Sensor to MCU)
  - UART (MCU to PC terminal)
- **Power**: USB

## 📁 Folder Structure

```
Core/
├── Inc/
│   └── main.h
├── Src/
│   └── main.c
Drivers/
├── MPU6050.h
├── MPU6050.c
Startup/
├── startup_stm32f401xe.s
```

## ⚙️ Functionality

- Initializes MPU6050 with ±2g acceleration range and 125 Hz sample rate.
- Reads raw accelerometer values (X, Y, Z).
- Calculates acceleration magnitude:  
  `m = sqrt(ax² + ay² + az²)`
- Compares difference from previous magnitude. If Δm > threshold (7000) and 400ms have passed, counts as one step.
- Sends `Step Count: X` messages via UART every 1 second.

## 📦 Dependencies

- STM32Cube HAL drivers (I2C, UART, GPIO, etc.)
- `math.h`, `stdio.h` for computation and UART formatting

## 🧪 Example UART Output

```
Step Counter Starting...
Step Count: 1
Step Count: 2
Step Count: 3
...
```

## 🚫 Limitations

- Fixed threshold (not adaptive)
- No noise filtering (basic debounce with 400ms delay)
- Walking-only detection; other motions not classified

## 🚀 Future Improvements

- Add Kalman or moving average filter
- Adaptive thresholding per user
- Use gyroscope for motion classification
- Add BLE module for wireless data transmission

## 📄 License

This project is provided AS-IS under the terms described in the original STM32Cube license. See the LICENSE file if available.