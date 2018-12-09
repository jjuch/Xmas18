#include <WiFi.h>
#include <Servo.h>
#include <HTTPClient.h>

Servo myservo; // create servo object to control a servo
static const int servoPin = 13;

// WiFi connections
const char* ssid = "WiFi-2.4-E400";
const char* password = "JasperJasmijnRonaldRaven";

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
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    
    http.begin("http://users.ugent.be/~jjuchem/Extra/index.html");
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }
    else {
      Serial.println("Error on HTTP Request");
    }

    http.end();
  }
  delay(1000);
}
