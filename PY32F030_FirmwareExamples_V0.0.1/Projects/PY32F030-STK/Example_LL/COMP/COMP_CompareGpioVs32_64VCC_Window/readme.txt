================================================================================
                                样例使用说明
                                Sample Description
================================================================================
功能描述：
此样例演示了COMP比较器的window功能，比较器1负端用比较器2的负端(PA3)作为输入，
VREFCMP作为比较器正端输入，当PA3的电压值小于1.65V时,LED灯灭，大于1.65V时,LED灯亮。

Function descriptions:
This example demonstrates the window function of the COMP. The negative terminal
of the COMP1 is connected the negative terminal of the COMP2(PA3).VREFCMP used 
as the positive terminal of the COMP1.When the voltage value of PA3 is greater 
than 1.65V, the LED is on, and when it is less than 1.65V, the LED is off.
================================================================================
测试环境：
测试用板：PY32F031_STK
MDK版本： 5.28
IAR版本： 9.20
GCC 版本：GNU Arm Embedded Toolchain 10.3-2021.10

Test environment:
Test board: PY32F031_STK
MDK Version: 5.28
IAR Version: 9.20
GCC Version: GNU Arm Embedded Toolchain 10.3-2021.10
================================================================================
使用步骤：
1. 编译下载程序到MCU，并运行；
2. PA3输入大于1.65V电压，LED灯亮；
3. PA3输入小于1.65V电压，LED灯灭。

Example execution steps:
1. Compile and download the program and run it
2. when the PA3 input is greater than 1.65V,LED on
3. When the PA3 input is less than 1.65V,LED off
================================================================================
注意事项：

Notes:

================================================================================