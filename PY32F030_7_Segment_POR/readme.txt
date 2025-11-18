================================================================================
                              Sample Description
================================================================================
Function Description:
This sample demonstrates the use of GPIO and 7-segment display control. A 2-digit 
7-segment display is connected to the MCU. Pins A8, A9, and A10 are connected to 
LEDs that blink during operation. When the user presses the button connected to A1, 
counting starts from 0 to 99 on the 7-segment display. Pressing the button connected 
to A2 stops the counting.

================================================================================
Test Environment:
Test Board: PY32F030_AssemCorp_DevKit  
MDK Version: 5.28  
IAR Version: 9.20  
GCC Version: GNU Arm Embedded Toolchain 10.3-2021.10
================================================================================

Execution Steps:
1. Compile and flash the program to the development board.  
2. Press the button connected to A1 to start the counter.  
3. Observe the 0-99 counting on the 2-digit 7-segment display.  
4. Press the button connected to A2 to stop the counter.  
5. During counting, the LEDs connected to A8, A9, and A10 will blink.

================================================================================

Pin Mapping:
Segments:  
A  - A5  
B  - B0  
C  - B1  
D  - A15  
E  - A12  
F  - A6  
G  - A3  
DP - A4  

Digit Selection:  
Digit 1 - A11  
Digit 2 - A7  

Buttons:  
Start Button - A1  
Stop Button  - A2  
LEDs: A8, A9, A10
========================================================
