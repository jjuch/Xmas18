#include <WiFi.h>
#include <Servo.h>

Servo myservo; // create servo object to control a servo
static const int servoPin = 13;

// WiFi connections
const char* ssid = "WiFi-2.4-E400";
const char* password = "JasperJasmijnRonaldRaven";

WiFiServer server(80);

String header; // variable to store the HTTP request

String valueString = String(5); // decode HTTP GET value

int pos1 = 0;
int pos2 = 0;

void setup() {
  Serial.begin(115200); // start console
  
  myservo.attach(servoPin); // attaches the servo on servoPin to the servo object

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
            client.println("</head><body><h1>ESP32 with Servo</h1>");
            client.println("<p>Position: <span id=\"servoPos>\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");

            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.oninput;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");

            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");

            client.println("</body></html>");

            if (header.indexOf("GET /?value=")>=0) { // header: // GET /?value=180& HTTP/1.1
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);

              // rotate servo

              myservo.write(valueString.toInt());
              Serial.println(valueString);
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
