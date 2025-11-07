================================================================================
                                样例使用说明
                                Sample Description
================================================================================
功能描述：
此样例实现了TIM1和TIM2级联成48位计数器，TIM2做主机，TIM2的计数溢出信号作为TIM1的
输入时钟，通过配置TIM1和TIM2的重载寄存器值，（在TIM1中断回调函数中）实现LED灯以
1Hz频率闪烁。

Function descriptions:
This example realizes the cascade of TIM1 and TIM2 into a 48-bit counter, with 
TIM2 as the host.The count overflow signal of TIM2 acts as the input clock of 
TIM1.By configuring the reloaded register values of TIM1 and TIM2, the LED 
is toggled at 1Hz (in the TIM1 interrupt callback function).
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
1. 编译下载程序并运行；
2. LED灯以1Hz频率闪烁

Example execution steps:
1. compile and download the program to MCU and run it;
2. The LED blink at 1Hz
================================================================================
注意事项：
此例程计算方式为TIM2_ARR*TIM2_PSC*TIM1_ARR*TIM1_PSC/时钟
=800*100*100*1/8000000=1Hz

Notes:
the calculation method of example : TIM2_ARR*TIM2_PSC*TIM1_ARR*TIM1_PSC/ clock
=800*100*100*1/8000000=1Hz
================================================================================