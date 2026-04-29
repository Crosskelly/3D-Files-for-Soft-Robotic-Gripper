import processing.serial.*;
import controlP5.*;

ControlP5 cp5;

Serial myPort;  
static String val;    
int sensorVal = 0;

// Display
int serialSignal = 100;
String statusText = "Initializing...";
String currentAction = "HOLD"; // New variable for the action label
float circleColor;
boolean sideProfile = false;
int sidePannel = 1;

// Sensors
float A0, A1, A2, A3;
float pPower, vPower; // Changed D1/Lock to match Pressure and Vacuum status
String[] rawValues;

void setup() {
  size(600, 400);
  noStroke();
  
  String portName = "COM3"; 
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
  
  cp5 = new ControlP5(this);

  // Button to INFLATE (Fill)
  cp5.addButton("FILL")
     .setPosition(40, 220)
     .setSize(80, 30)
     .setColorBackground(color(0, 150, 0)); // Green

  // Button to VACUUM (Deflate)
  cp5.addButton("VACUUM")
     .setPosition(40, 260)
     .setSize(80, 30)
     .setColorBackground(color(150, 0, 0)); // Red

  // Button to STOP (Hold)
  cp5.addButton("STOP")
     .setPosition(40, 300)
     .setSize(80, 30)
     .setColorBackground(color(100)); // Grey
}

void draw() {
  background(0);
  
  // Request data from Arduino
  myPort.write("R"); 
  
  // Visual Indicator for Serial Heartbeat (fades over time)
  if (serialSignal > 100) serialSignal -= 5;
  
  // STATUS DISPLAY
  fill(255);
  text("System Status: " + statusText, 20, 30);
  text("Current Action: " + currentAction, 20, 50);
  text("Sensors: " + A0 + " | " + A1 + " | " + A2 + " | " + A3, 20, 80);
  text("Valves: [P: " + pPower + "] [V: " + vPower + "]", 20, 100);
  
  fill(serialSignal, 100, 100);
  ellipse(150, 45, 12, 12);
  
  // GRIPPER VISUALIZATION
  if (!sideProfile) {
    fill(map(A0, 600, 1000, 0, 255), 255, 255);
    rect(475, 190, 70, 30);
    fill(map(A1, 600, 1000, 0, 255), 255, 255);
    rect(250, 190, 70, 30);
    fill(map(A2, 600, 1000, 0, 255), 255, 255);
    rect(375, 50, 30, 70);
    
    circleColor = map(A3, 600, 1000, 0, 255);
    fill(circleColor, 100, 100);
    ellipse(400, height/2, 100, 100);
  }
}

// Controller events to send commands to Arduino
public void FILL() {
  myPort.write('O'); // 'O' for Open/Fill
}

public void VACUUM() {
  myPort.write('V'); // 'V' for Vacuum
}

public void STOP() {
  myPort.write('S'); // 'S' for Stop/Hold
}

void serialEvent(Serial myPort) {
  String inString = myPort.readStringUntil('\n'); 
  if (inString != null) {
    inString = trim(inString);
    rawValues = split(inString, ',');
    
    // We expect 7 values: A0, A1, A2, A3, pPower, vPower, actionName
    if (rawValues.length >= 7) {
      serialSignal = 255; // Flash the signal light
      
      A0 = float(rawValues[0]);
      A1 = float(rawValues[1]);
      A2 = float(rawValues[2]);
      A3 = float(rawValues[3]);
      pPower = float(rawValues[4]);
      vPower = float(rawValues[5]);
      currentAction = rawValues[6];
      
      statusText = "Connected";
    } else {
      statusText = "Data Format Error (" + rawValues.length + ")";
    }
  }
}
