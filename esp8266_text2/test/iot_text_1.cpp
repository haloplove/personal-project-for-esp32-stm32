#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "ESP-text-CAR";
const char *password = "12345611";

ESP8266WebServer server(80);
String webPage = "";

int gpio5_pin = 5; // a1 motorcontroller
int gpio4_pin = 4; // a2
int gpio0_pin = 0; // b1
int gpio2_pin = 2; // b2
int flagf = 0, flagb = 0, flagr = 0, flagl = 0;
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot()
{
    server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup()
{
    // webPage += "<div align=\"center\"><h1>ESP8266 R/C CAR</h1>";
    // webPage += "<a href=\"FW\"><button style=\"height:200px;width:200px\">FW</button></a><br>";
    // webPage += "<a href=\"LT\"><button style=\"height:200px;width:200px\">LT</button></a>";
    // webPage += "<a href=\"STOP\"><button style=\"height:200px;width:200px\">STOP</button></a>";
    // webPage += "<a href=\"RT\"><button style=\"height:200px;width:200px\">RT</button></a><br>";
    // webPage += "<a href=\"BW\"><button style=\"height:200px;width:200px\">BW</button></a></div>";

    webPage += "<div align=\"center\"><h1>ESP8266 CAR TEXT</h1>";
    webPage += "<a href=\"FL\"><button style=\"height:200px;width:200px\"><font size=\"20\">FL</font></button></a>";
    webPage += "<a href=\"F2\"><button style=\"height:200px;width:200px\"><font size=\"20\">FW</font></button></a>";
    webPage += "<a href=\"FR\"><button style=\"height:200px;width:200px\"><font size=\"20\">FR</font></button></a><br>";
    webPage += "<a href=\"L2\"><button style=\"height:200px;width:200px\"><font size=\"20\">LT</font></button></a>";
    webPage += "<a href=\"S2\"><button style=\"height:200px;width:200px\"><font size=\"20\">ST</font></button></a>";
    webPage += "<a href=\"R2\"><button style=\"height:200px;width:200px\"><font size=\"20\">RT</font></button></a><br>";
    webPage += "<a href=\"BL\"><button style=\"height:200px;width:200px\"><font size=\"20\">BL</font></button></a>";
    webPage += "<a href=\"B2\"><button style=\"height:200px;width:200px\"><font size=\"20\">BC</font></button></a>";
    webPage += "<a href=\"BR\"><button style=\"height:200px;width:200px\"><font size=\"20\">BR</font></button></a></div>";
    webPage += "<div align=\"center\"><h1>SPEED CHANGE</h1>";
    webPage += "<a href=\"SE1\"><button style=\"height:200px;width:200px\"><font size=\"20\">1</font></button></a>";
    webPage += "<a href=\"SE2\"><button style=\"height:200px;width:200px\"><font size=\"20\">2</font></button></a>";
    webPage += "<a href=\"SE3\"><button style=\"height:200px;width:200px\"><font size=\"20\">3</font></button></a><br>";
    webPage += "<a href=\"SE4\"><button style=\"height:200px;width:200px\"><font size=\"20\">4</font></button></a>";
    webPage += "<a href=\"SE5\"><button style=\"height:200px;width:200px\"><font size=\"20\">5</font></button></a>";
    webPage += "<a href=\"SE6\"><button style=\"height:200px;width:200px\"><font size=\"20\">6</font></button></a><br>";
    webPage += "<a href=\"SE7\"><button style=\"height:200px;width:200px\"><font size=\"20\">7</font></button></a>";
    webPage += "<a href=\"SE8\"><button style=\"height:200px;width:200px\"><font size=\"20\">8</font></button></a>";
    webPage += "<a href=\"SE9\"><button style=\"height:200px;width:200px\"><font size=\"20\">9</font></button></a>";

    // preparing GPIOs
    pinMode(gpio5_pin, OUTPUT);
    digitalWrite(gpio5_pin, LOW);
    pinMode(gpio4_pin, OUTPUT);
    digitalWrite(gpio4_pin, LOW);
    pinMode(gpio0_pin, OUTPUT);
    digitalWrite(gpio0_pin, LOW);
    pinMode(gpio2_pin, OUTPUT);
    digitalWrite(gpio2_pin, LOW);
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.on("/", []()
              { server.send(200, "text/html", webPage); });
    server.on("/FW", []()
              {
    server.send(200, "text/html", webPage);
    if (flagf == 0)
    {
      digitalWrite(gpio2_pin, LOW);
      digitalWrite(gpio0_pin, HIGH);
      flagf = 1;
    }
    else
    {
      digitalWrite(gpio2_pin, LOW);
      digitalWrite(gpio0_pin, LOW);
      flagf = 0;
    }
    delay(1000); });
    server.on("/BW", []()
              {
    server.send(200, "text/html", webPage);
    if (flagb == 0)
    {
      digitalWrite(gpio2_pin, HIGH);
      digitalWrite(gpio0_pin, LOW);
      flagb = 1;
    }
    else
    {
      digitalWrite(gpio2_pin, LOW);
      digitalWrite(gpio0_pin, LOW);
      flagb = 0;
    }
     delay(1000); });
    server.on("/STOP", []()
              {
    server.send(200, "text/html", webPage);
    digitalWrite(gpio2_pin, LOW);
    digitalWrite(gpio0_pin, LOW);
    digitalWrite(gpio4_pin, LOW);
    digitalWrite(gpio5_pin, LOW);
    flagf = flagb = flagr = flagl = 0;
     delay(1000); });
    server.on("/LT", []()
              {
    server.send(200, "text/html", webPage);
    if (flagl == 0)
    {
      digitalWrite(gpio4_pin, HIGH);
      digitalWrite(gpio5_pin, LOW);
      flagl = 1;
    }
    else
    {
      digitalWrite(gpio4_pin, LOW);
      digitalWrite(gpio5_pin, LOW);
      flagl = 0;
    }
     delay(1000); });
    server.on("/RT", []()
              {
            server.send(200, "text/html", webPage);
            if (flagr == 0)
            {
              digitalWrite(gpio4_pin, LOW);
              digitalWrite(gpio5_pin, HIGH);
              flagr = 1;
            }
            else
            {
              digitalWrite(gpio4_pin, LOW);
              digitalWrite(gpio5_pin, LOW);
              flagr = 0;
            }
             delay(1000); });
    server.begin();
    Serial.println("HTTP server started");
}

void loop()
{
    server.handleClient();
}