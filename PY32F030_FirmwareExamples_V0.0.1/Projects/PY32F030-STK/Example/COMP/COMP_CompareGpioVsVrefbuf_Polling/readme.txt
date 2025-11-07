================================================================================
                                样例使用说明
                             Sample Description
================================================================================
功能描述：
此样例演示了COMP比较器轮询功能，PA1作为比较器正端输入，Vrefbuf2.5V作为比较器负端
输入，当PA1的电压大于Vrefbuf电压时，LED灯亮，小于Vrefbuf电压时,LED灯灭。

Function descriptions:
This example demonstrates the COMP comparator polling function, with PA1 as the
positive input and Vrefbuf2.5V as the negative input. When the voltage of PA1
is greater than Vrefbuf voltage, the LED lights up, and when it is less than
Vrefbuf voltage, the LED light off.
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
2. 配置PA1输入2.6V电压，LED灯亮
3. 配置PA1输入2.4V电压, LED灯灭

Example execution steps:
1.Compile and download the program to MCU and run it;
2.Configure PA1 input voltage of 2.6V, LED light on
3.Configure PA1 input voltage of 2.4V, LED light off
================================================================================
注意事项：

Notes:

================================================================================