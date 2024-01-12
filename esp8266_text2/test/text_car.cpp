/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : 3_2_2_Turning_on_and_off_an_LED
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20191108
程序目的/Purpose          : 使用NodeMCU建立基本服务器。用户可通过浏览器使用8266的IP地址
                           访问8266所建立的基本网页并通过该页面点亮/熄灭NodeMCU的内置LED
-----------------------------------------------------------------------
修订历史/Revision History
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description

***********************************************************************/
// #include <ESP8266WiFi.h>      // 本程序使用 ESP8266WiFi库
// #include <ESP8266WiFiMulti.h> //  ESP8266WiFiMulti库
// #include <ESP8266WebServer.h> //  ESP8266WebServer库

// ESP8266WiFiMulti wifiMulti; // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'

// ESP8266WebServer esp8266_server(80); // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

// void handleRoot();
// void handleLED();
// void handleNotFound();

// void setup(void)
// {
//   Serial.begin(9600); // 启动串口通讯

//   pinMode(LED_BUILTIN, OUTPUT); //设置内置LED引脚为输出模式以便控制LED

//   wifiMulti.addAP("altman", "ysatmxhs");                       // 将需要连接的一系列WiFi ID和密码输入这里
//   wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
//   wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
//   Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。

//   int i = 0;
//   while (wifiMulti.run() != WL_CONNECTED)
//   {              // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
//     delay(1000); // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
//     Serial.print(i++);
//     Serial.print(' '); // 将会连接信号最强的那一个WiFi信号。
//   }                    // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
//                        // 此处while循环判断是否跳出循环的条件。

//   // WiFi连接成功后将通过串口监视器输出连接成功信息
//   Serial.println('\n');
//   Serial.print("Connected to ");
//   Serial.println(WiFi.SSID()); // 通过串口监视器输出连接的WiFi名称
//   Serial.print("IP address:\t");
//   Serial.println(WiFi.localIP()); // 通过串口监视器输出ESP8266-NodeMCU的IP

//   esp8266_server.begin();                          // 启动网站服务
//   esp8266_server.on("/", HTTP_GET, handleRoot);    // 设置服务器根目录即'/'的函数'handleRoot'
//   esp8266_server.on("/LED", HTTP_POST, handleLED); // 设置处理LED控制请求的函数'handleLED'
//   esp8266_server.onNotFound(handleNotFound);       // 设置处理404情况的函数'handleNotFound'

//   Serial.println("HTTP esp8266_server started"); //  告知用户ESP8266网络服务功能已经启动
// }

// void loop(void)
// {
//   esp8266_server.handleClient(); // 检查http服务器访问
// }

// /*设置服务器根目录即'/'的函数'handleRoot'
//   该函数的作用是每当有客户端访问NodeMCU服务器根目录时，
//   NodeMCU都会向访问设备发送 HTTP 状态 200 (Ok) 这是send函数的第一个参数。
//   同时NodeMCU还会向浏览器发送HTML代码，以下示例中send函数中第三个参数，
//   也就是双引号中的内容就是NodeMCU发送的HTML代码。该代码可在网页中产生LED控制按钮。
//   当用户按下按钮时，浏览器将会向NodeMCU的/LED页面发送HTTP请求，请求方式为POST。
//   NodeMCU接收到此请求后将会执行handleLED函数内容*/
// void handleRoot()
// {
//   esp8266_server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
// }

// //处理LED控制请求的函数'handleLED'
// void handleLED()
// {
//   digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // 改变LED的点亮或者熄灭状态
//   esp8266_server.sendHeader("Location", "/");           // 跳转回页面根目录
//   esp8266_server.send(303);                             // 发送Http相应代码303 跳转
// }

// // 设置处理404情况的函数'handleNotFound'
// void handleNotFound()
// {
//   esp8266_server.send(404, "text/plain", "404: Not found"); // 发送 HTTP 状态 404 (未找到页面) 并向浏览器发送文字 "404: Not found"
// }
/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : 3_2_3_Pin_State_Display
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20191107
程序目的/Purpose          : 使用NodeMCU建立基本服务器。该页面将会自动刷新并且显示NodeMCU
                           的D3引脚状态。NodeMCU开发板上的FLASH按键可以控制D3引脚的电平。
                           没有按下该按键时D3引脚将会保持高电平状态。当按下该按键后，
                           D3引脚会变为低电平。
-----------------------------------------------------------------------
修订历史/Revision History
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
***********************************************************************/

#include <ESP8266WiFi.h>      // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h> //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h> //  ESP8266WebServer库

#define D3 GPIO_ID_PIN0
#define buttonPin D3 // 按钮引脚D3

void handleRoot();
void handleNotFound();

ESP8266WiFiMulti wifiMulti; // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'

ESP8266WebServer esp8266_server(80); // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

bool pinState; // 存储引脚状态用变量

void setup()
{
    Serial.begin(115200); // 启动串口通讯
    Serial1.begin(115200);

    pinMode(buttonPin, INPUT_PULLUP); // 将按键引脚设置为输入上拉模式

    wifiMulti.addAP("altman", "ysatmxhs");                       // 将需要连接的一系列WiFi ID和密码输入这里
    wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
    wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
    Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。

    int i = 0;
    while (wifiMulti.run() != WL_CONNECTED)
    {                // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
        delay(1000); // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
        Serial.print(i++);
        Serial.print(' '); // 将会连接信号最强的那一个WiFi信号。
    }                      // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                           // 此处while循环判断是否跳出循环的条件。
    // WiFi连接成功后将通过串口监视器输出连接成功信息
    Serial.println('\n');           // WiFi连接成功后
    Serial.print("Connected to ");  // NodeMCU将通过串口监视器输出。
    Serial.println(WiFi.SSID());    // 连接的WiFI名称
    Serial.print("IP address:\t");  // 以及
    Serial.println(WiFi.localIP()); // NodeMCU的IP地址

    esp8266_server.begin();                    // 启动网站服务
    esp8266_server.on("/", handleRoot);        // 设置服务器根目录即'/'的函数'handleRoot'
    esp8266_server.onNotFound(handleNotFound); // 设置处理404情况的函数'handleNotFound'

    Serial.println("HTTP esp8266_server started"); //  告知用户ESP8266网络服务功能已经启动
}

char cmd = 0;
void loop()
{
    esp8266_server.handleClient();     // 处理http服务器访问
    pinState = digitalRead(buttonPin); // 获取引脚状态
    cmd = Serial1.read();
    if (cmd != '0')
    {
        // Serial.write(cmd);
        // delay(500);

        if (cmd == '5')
        {
            Serial.write('5');
        }
        else if (cmd == '6')
        {
            Serial.write('6');
        }
        else if (cmd == '8')
        {
            Serial.write('8');
        }
        else if (cmd == '9')
        {
            Serial.write('9');
        }
    }
}

/* 以下函数处理网站首页的访问请求。此函数为本示例程序重点1
详细讲解请参见太极创客网站《零基础入门学用物联网》
第3章-第2节“通过网络服务将开发板引脚状态显示在网页中”的说明讲解。*/
void handleRoot()
{
    String displayPinState; // 存储按键状态的字符串变量

    if (pinState == HIGH)
    {                                           // 当按键引脚D3为高电平
        displayPinState = "Button State: HIGH"; // 字符串赋值高电平信息
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {                                          // 当按键引脚D3为低电平
        displayPinState = "Button State: LOW"; // 字符串赋值低电平信息
        digitalWrite(LED_BUILTIN, LOW);
    }

    esp8266_server.send(200, "text/plain", displayPinState);
    // 向浏览器发送按键状态信息
}

// 设置处理404情况的函数'handleNotFound'
void handleNotFound()
{                                                             // 当浏览器请求的网络资源无法在服务器找到时，
    esp8266_server.send(404, "text/plain", "404: Not found"); // NodeMCU将调用此函数。
}

// #include "Arduino.h"

// void setup()
// {
//   // initialize LED digital pin as an output.
//   pinMode(LED_BUILTIN, OUTPUT);
// }

// void loop()
// {
//   // turn the LED on (HIGH is the voltage level)
//   digitalWrite(LED_BUILTIN, HIGH);
//   // wait for a second
//   delay(1000);
//   // turn the LED off by making the voltage LOW
//   digitalWrite(LED_BUILTIN, LOW);
//   // wait for a second
//   delay(1000);
// }