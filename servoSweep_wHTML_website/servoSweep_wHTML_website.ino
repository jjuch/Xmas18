#include <WiFi.h>
#include <Servo.h>
/*#include <ESPAsyncWebServer.h>*/

Servo myservo1; // create servo object to control a servo
Servo myservo2;

static const int servoPin1 = 13;
static const int servoPin2 = 12;


// WiFi connections
/*const char* ssid = "WiFi-2.4-E400";
const char* password = "JasperJasmijnRonaldRaven";*/
const char* ssid = "WiFi-2.4-E119";
const char* password = "D67A41FB1A";

WiFiServer server(80);

String header; // variable to store the HTTP request

String valueString1 = String(90); // decode HTTP GET value
String valueString2 = String(90); // decode HTTP GET value

int pos1 = 0;
int pos2 = 0;
int pos3 = 0;

void setup() {
  Serial.begin(115200); // start console
  
  myservo1.attach(servoPin1); // attaches the servo on servoPin to the servo object
  myservo2.attach(servoPin2);

  /*myservo1.write(20);
  myservo2.write(20);

  delay(3000);*/

  myservo1.write(15);
  myservo2.write(15);

  // connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected: ");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // listen for incoming clients

  if (client) { // if a new client connects
    Serial.println("New Client.");
    String currentLine = ""; // init string to hold incoming data from client

    while (client.connected()) { // loop while client is connected
      if (client.available()) { // if there are bytes to read from the client
        /*myservo1.write(90);
        myservo2.write(90);*/
        char c = client.read();
        Serial.write(c);
        header += c;
        
        if (c=='\n') { // if the byte is a newline character
          // if the current line is blank, you get two newline characters in a row. That is the end of the client HTTP request  to send a response:
          if (currentLine.length() == 0){ // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK) and a content-type trigger the client, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // display HTML page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            // CSS
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; Margin-left: auto; margin-right: auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");

            // web page
            client.println("</head><body><h1>Krijg je het cadeautje open?</h1><h2>Vul de juiste combinatie in:</h2><h3>Slot 1</h3>");
            client.println("<p>Position: <span id=\"servoPos1\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider1\" onchange=\"servo(this.value,1)\" value=\""+valueString1+"\"/>");
            client.println("<h3>Slot 2</h3><p>Position: <span id=\"servoPos2\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider2\" onchange=\"servo(this.value,2)\" value=\""+valueString2+"\"/>");

            client.println("<script type =\"text/javascript\">var slider1 = document.getElementById(\"servoSlider1\");");
            client.println("var slider2 = document.getElementById(\"servoSlider2\");");
            client.println("var servoP1 = document.getElementById(\"servoPos1\"); servoP1.innertext = ''+slider1.value;");
            client.println("var servoP2 = document.getElementById(\"servoPos2\"); servoP2.innertext = ''+slider2.value;");
            
            client.println("slider1.oninput = function() { slider1.value = this.value; servoP1.innerHTML = this.value; }");
            client.println("slider2.oninput = function() { slider2.value = this.value; servoP2.innerHTML = this.value; }");

            client.println("$.ajaxSetup({timeout:1000}); function servo(pos, sliderNr) {let pos1=0; let pos2=0; if (sliderNr ==1) {pos1=pos;pos2=slider2.value;} else {pos1=slider1.value; pos2=pos;};");
            client.println("$.get(\"/?value=\" + pos1 + \"!\" + pos2 + \"&\"); {Connection: close};}</script>");

            client.println("</body></html>");

            if (header.indexOf("GET /?value=")>=0) { // header: // GET /?value=180& HTTP/1.1
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('!');
              pos3 = header.indexOf('&');
              valueString1 = header.substring(pos1+1, pos2);
              valueString2 = header.substring(pos2+1, pos3);

              // rotate servo

              int val1=valueString1.toInt();
              int val2=valueString2.toInt();

              Serial.println(val1);
              Serial.println(val2);

              if (val1 == 161) {
                Serial.print("1 OK");
                myservo1.write(15);
              }
              else {
                Serial.print("1 NOK");
                myservo1.write(90);
              }
              
              if (val2 == 20) {
                Serial.print("2 OK");
                myservo2.write(15);
              }
              else {
                Serial.print("2 NOK");
                myservo2.write(90);
              }
            }

            // the HTTP response ends with another blank line
            client.println();
            
            break; // break out of the while loop
                        
          } else { // if you got a newline -> clear currentLine
            currentLine = "";
        }
      }else if (c != '\r') { // if you get anything else but a carriage return character, add it to end of currentLine
        currentLine += c;
      }      
    }
  }

  // clear header variable
  header = "";

  // close connection
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
  }
}
