# **WIFI网络与红外编码桥接器**
&nbsp;&nbsp;***本项目程序代码已经通过实测认证，可直接移植！***   
&nbsp;&nbsp;利用手机APP程序，通过WIFI发送一些控制命令程序。  
&nbsp;&nbsp;在STM32开发板通过ESP8266接收，然后使用红外管发现指令  
&nbsp;&nbsp;家用红外电器家具接收到指令，进行相应动作。  
&nbsp;&nbsp;以此，通过此WIFI网络与红外编码桥接器，实现手机控制非智能联网电器。

## **程序解释**
//程序功能：旧的电器不能使用手机控制，现编写程序让旧电器也可以WIFI控制  
//编写作者：Mark Zhao  
//编写时间：2020.01.05  
//修改记录：暂无。  

## **解决问题**
解决问题：新的智能空调可以使用手机通过WIFI来控制，而旧的空调则不行。  
&nbsp; &nbsp;&nbsp; &nbsp;  通过一块小的电路板，通过WIFI桥接手机，同时可以发出红外信号。  
&nbsp; &nbsp; &nbsp; &nbsp; 这样通过编程就可以使用手机近距离或远程控制旧空调”秒变智能“。  

## **实现思路**
实现思路：使用一块STM32开发板，安装红外发射二极管，按红外接收电器的编码规则发出红外信号。  
最重要的是，该开发板上安装ESP8266 WIFI模块，辐射出一个wifi信号，手机可以连接wifi信号，  
通过APP控制开发板发出规则的红外编码信号，红外电器接收到信号进行处理。也可以连接家里的路由，  
通过手机APP远程控制。

## **实现过程**
实现过程：浮生偷得半日闲，今天周日休息一天，利用这一天时间编写好所有程序。  
为便于所有读者哪怕是学生朋友理解，本程序在正点原子的基础上进行二次开发。  
循序渐进，分十个过程（8小时完成）。  
- 01 ESP8266WIFI模块控制程序（1小时）  
- 02 定时器中断_每500msLED亮（10分钟）  
- 03 定时器输出可调占空比PWM（10分钟）  
- 04 输入捕获实验（10分钟）  
- 05 红外遥控实验_战舰F103_标准库（10分钟）  
- 06 红外遥控器实验_阿波罗F429_HAL库（10分钟）  
- 07 红外发射程序（2小时）  
- 08 串口网络助手调试控制LED亮灭（10分钟）  
- 09 手机APP程序（2小时）  
- 10 综合联调__手机控制__旧电器秒变智能（2小时）  

## **实现结果**
实现结果：  
1、战舰开发板上电就开机，直接辐射出WIFI网络，手机电脑均可连接。  
2、手机作为客户端直接可以发送控制码。  
3、战舰开发板收到控制码后，LED1亮代表收到，然后发出红外编码信号。  
4、阿波罗开发板收到红外信号后，LED1亮代表接收正常。  
5、同理也实现了LED1的灭。  

## **详细内容**
详细内容请见本开源程序的“4.doc"文件描述。