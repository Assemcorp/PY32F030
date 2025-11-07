# AI Agent Instructions for PY32F031 GPIO Example

## Project Overview
This is a firmware example for the PY32F031 microcontroller showcasing GPIO functionality. The project demonstrates LED control using GPIO output mode.

## Project Structure
- `Src/main.c`: Main application logic
- `Inc/`: Header files containing configurations and declarations
- Multiple build system support:
  - MDK-ARM/: Keil MDK project files
  - EWARM/: IAR project files
  - EIDE/: Embedded IDE project files

## Key Concepts and Patterns

### Hardware Abstraction Layer (HAL)
- Uses PY32F031 HAL for peripheral access
- GPIO configuration follows HAL initialization pattern:
  1. Enable GPIO clock
  2. Configure GPIO parameters (mode, pull, speed)
  3. Initialize GPIO using HAL_GPIO_Init()

### GPIO Pin Conventions
- LED pin definitions are in header files
- Standard pattern for GPIO configuration:
```c
#define LED_GPIO_PIN                 LED3_PIN
#define LED_GPIO_PORT                LED3_GPIO_PORT
#define LED_GPIO_CLK_ENABLE()        LED3_GPIO_CLK_ENABLE()
```

## Build Environment
Supported toolchains:
- MDK-ARM v5.28
- IAR v9.20
- GCC (GNU Arm Embedded Toolchain 10.3-2021.10)

## Development Workflow
1. Select your preferred toolchain (MDK/IAR/GCC)
2. Open the corresponding project file:
   - MDK: `MDK-ARM/Project.uvprojx`
   - IAR: `EWARM/Project.ewp`
   - EIDE: `EIDE/Project.code-workspace`
3. Compile and flash to PY32F031-STK board

## Testing
- Connect PY32F031-STK board
- After flashing, LED3 should blink at 2Hz frequency (toggles every 250ms)
- Use debugger through selected IDE for real-time monitoring

## License
This project is licensed under BSD 3-Clause license. See license text in source file headers.