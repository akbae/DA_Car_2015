/*
 * Written by: R-STEM (Austin Bae) with template from Vasilakis Michalis
 * Created: 6-5-2015, Last edited: 7-8-2015
 * Project:  Bluetooth RC Car
 *           Direction: 8-direction pad around 's' key
 *           Variable motor speed: '1'-'4'
 *           Lights: Turn signals, brake lights, headlights, anything
 *                   programmed in any way that you want!
 *           Buzzer: Horn that plays in any pattern that you would like
 */


// Motors - IC pins
const int leftBackward  = 5;  // Pin  2 of L293
const int leftForward  = 6;  // Pin  7 of L293
const int rightBackward  = 10; // Pin 10 of L293
const int rightForward  = 11;  // Pin 14 of L293

// Light pins
const int leftLight = 8;
const int rightLight = 9;

// Buzzer pin
const int buzzer = 3;

// State provided by serial monitor/bluetooth activates car
char state = 's'; // default = 's' = stop

// Motor speed (0-255)
int vSpeed = 255; // Default speed

// Array of possible direction keys
char directions[] = {'q','w','e','a','d','z','x','c'};

// Counter to keep direction for mo\re fluid motion
int directionCount = 0;

// Counter/status check for blinking
int lightCount = 0;
bool leftOn = 0;
bool rightOn = 0;

// Counter/status check for buzzing
bool buzzOn = 0;
int buzzCount = 0;


void setup() {
  
  // Outputs
  pinMode(leftBackward, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(rightBackward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  
  pinMode(leftLight, OUTPUT);
  pinMode(rightLight, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Communication w/ Serial Monitor = 9600, w/ BT = 115200
  Serial.begin(115200);
  
}//setup

void loop() {
  
  // Stops car when no commands given
  if (!in(directions,state) || directionCount >= 15){
    state = 's';
    directionCount = 0; //
  }//if ! a direction command or loop has iterated 15x with direction command
  else{
    directionCount++;
  }//else
  
  // Communication with serial monitor/bluetooth
  if (Serial.available() > 0) {
    state = Serial.read();
  }//if serial comunication is available

  // A switch statement checks its parameter (state) to see if it
  // equals the value following each 'case' keyword. In its basic
  // form, it is a series of if-statements.
  switch(state) {
  
  // Changes speed of the motors (1-4)
  case '1': vSpeed = 100;
            break;
  case '2': vSpeed = 150; 
            break;
  case '3': vSpeed = 200;
            break;
  case '4': vSpeed = 255; // Max speed
            break;
            
  // Left lights - 'r'
  case 'r': leftOn = !leftOn;
            digitalWrite(leftLight,LOW);
            break;
            
  // Right lights - 't'
  case 't': rightOn = !rightOn;
            digitalWrite(rightLight,LOW);
            break;

  // All lights
  case 'g': leftOn = 0;
            rightOn = 0;
            digitalWrite(rightLight,!digitalRead(rightLight));
            digitalWrite(leftLight,!digitalRead(leftLight));
            break;
  
  // Horn - 'f'
  case 'f': tone(buzzer, 440);//Speaker on
            buzzCount = 10;
            buzzOn = 1;
            break;

  // Forward - 'w'
  case 'w': analogWrite(leftForward, vSpeed);
            analogWrite(rightForward, vSpeed);
    
            analogWrite(leftBackward, 0);
            analogWrite(rightBackward, 0);
            break;
  
  // Forward left - 'q'
  case 'q': analogWrite(leftForward, vSpeed / 2);
            analogWrite(rightForward, vSpeed);
     
            analogWrite(leftBackward, 0); 
            analogWrite(rightBackward, 0);
            break;
  
  // Forward right - 'e'
  case 'e': analogWrite(leftForward, vSpeed);
            analogWrite(rightForward, vSpeed / 2);
    
            analogWrite(leftBackward, 0); 
            analogWrite(rightBackward, 0);
            break;
            
  // Backward - 'x'
  case 'x': analogWrite(leftForward, 0);
            analogWrite(rightForward, 0);
    
            analogWrite(leftBackward, vSpeed);
            analogWrite(rightBackward, vSpeed);   
            break;
  
  // Backward left - 'z'
  case 'z': analogWrite(leftForward, 0);
            analogWrite(rightForward, 0);
          
            analogWrite(leftBackward, vSpeed / 2);     
            analogWrite(rightBackward, vSpeed);   
            break;
  
  // Backward right - 'c'
  case 'c': analogWrite(leftForward, 0);
            analogWrite(rightForward, 0);
            
            analogWrite(leftBackward, vSpeed);   
            analogWrite(rightBackward, vSpeed / 2);
            break;
  
  // Left - 'a'
  case 'a': analogWrite(leftForward, 0);
            analogWrite(rightForward, vSpeed);
            
            analogWrite(leftBackward, 0);   
            analogWrite(rightBackward, 0); 
            break;
  
  // Right - 'd'
  case 'd': analogWrite(leftForward, vSpeed);
            analogWrite(rightForward, 0);
    
            analogWrite(leftBackward, 0);   
            analogWrite(rightBackward, 0);     
            break;

  // Stops the car - 's'
  case 's': analogWrite(leftForward, 0);
            analogWrite(rightForward, 0);
        
            analogWrite(leftBackward, 0);  
            analogWrite(rightBackward, 0);
            break;
            
  default:  Serial.println("Input error");
  
  }//switch
  
  // Light blink counter
  if (leftOn || rightOn){
    lightCount++;
    if (lightCount >= 20){
      if (leftOn){
        digitalWrite(leftLight,!digitalRead(leftLight));
      }//if left
      if (rightOn){
        digitalWrite(rightLight,!digitalRead(rightLight));
      }//if right
      lightCount = 0;
    }//if 20 iterations
  }//if left or right turn signal on
    
  // Buzz counter  
  if (buzzOn){
    buzzCount--;
  }//if buzz
  if (buzzCount < 0) {
    noTone(buzzer);
    buzzOn = 0;
  }//if buzzCount < 0
  
  delay(30); // Necessary for keydown to register
  
}//loop

// Function written for directions[]
// Checks if char check is found within array[]
bool in(char array[], char check){
  
   for (int x = 0; x < 8; x++){
     if (check == array[x]){
       return 1;
     }//if found
   }//for x
   
   return 0;

}//in
