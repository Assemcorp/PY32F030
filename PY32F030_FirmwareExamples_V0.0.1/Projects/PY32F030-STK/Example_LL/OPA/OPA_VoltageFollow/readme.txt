================================================================================
                                样例使用说明
                             Sample Description
================================================================================
功能描述：
此样例演示了OPA的电压跟随功能，PA9为正端输入，PA10为负端输入，PA8为输出，PA8会输
出和PA9相同的电压值。

Function descriptions:
This sample demonstrates the voltage follower functionality of the OPA. PA9 
is the positive input, PA10 is the negative input, and PA8 is the output. PA8 
will output the same voltage as PA9.
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
1. 编译并下载程序。
2. 将PA10和PA8短接。
3. 在PA9端输入1V、2V、3V，观察PA8端检测到的电压值，应与PA9端输入的电压值相同。

Example execution steps:
1. Compile and download the program.
2. Short PA10 and PA8 together.
3. Input 1V, 2V, and 3V at the PA9 pin. Observe the voltage detected at the 
PA8 pin, which should be the same as the input voltage at PA9.
================================================================================
注意事项：

Notes:

================================================================================