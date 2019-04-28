#include <Bridge.h>
#include <Process.h>
#include <Servo.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

BridgeServer server;
BridgeClient client;
Servo XServo;
Servo YServo;
Process picture;

String filename;
String path = "/mnt/sda1/";

void setup() {
  XServo.attach(9);
  YServo.attach(10);
  Bridge.begin();
  pinMode(8,INPUT);
  server.noListenOnLocalhost();
  server.begin();
}

void loop() {
  // Get clients coming from server
  BridgeClient client = server.accept();  
   
  // Is there a new client?  
  if (client) {    
    // Process request
    process(client);
    // Close connection and free resources.
    client.stop();
  }
  delay(50);
  
  if (digitalRead(8) == true) {
    filename = "";
    picture.runShellCommand("date +%s");
    while(picture.running());
   
    while (picture.available() > 0) {
      char c = picture.read();
      filename += c;
    } 
    filename.trim();
    filename += ".jpg";

    // Save the picture locally on a microSD located within the Arduino Yun
    picture.runShellCommand("fswebcam " + path + filename + " -r 1280x720");
    while(picture.running());
    Serial.println("File successfully saved locally!");

    // Upload to Amazon S3
    picture.runShellCommand("python " + path + "upload_picture.py " + path + filename);
    while(picture.running());
  }
}

void process(BridgeClient client) {
  // read the command  
  String command = client.readStringUntil('/');
  command.trim();
 
  if (command == "servo") {
       servoCommand(client);
  }
}

void servoCommand(BridgeClient client) {
  
  int pin, value;
  pin = client.parseInt();
  
  if(client.read() == '/') {
    value = client.parseInt();
    if (pin == 9) {
      XServo.write(value);  
    }
    else if (pin == 10) {
      YServo.write(value);
    }
  }
}
