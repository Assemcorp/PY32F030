================================================================================
                                样例使用说明
                             Sample Description
================================================================================
功能描述：
此样例演示了通过中断方式计算sqrt的值。

Function descriptions:
This example demonstrates calculating the value of sqrt through interruption.
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
1. 编译并下载程序；
2. 通过USB转TTL模块连接PC与STK板,STK板与USB转TTL模块的连线方式如下；
STK板        USB转TTL模块
PA02(TX)  -->  RX
PA03(RX)  -->  TX
GND       -->  GND
3. 程序运行后，观察打印的sqrt值（sqrt(16) = 4）

Example execution steps:
1. compile and download the program to MCU and run it;
2. Connect PC and STK board through USB to TTL module and the connection method
is as follows
STK board    USB to TTL moudle
PA02(TX)  -->  RX
PA03(RX)  -->  TX
GND      -->  GND
3. After the program runs, observe the printed sqrt value (sqrt(16)=4)
================================================================================
注意事项：

Notes:

================================================================================