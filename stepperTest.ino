/** Code for SideProject's adaptation of Samy Kamkar's Combo Breaker
 *  Notes:
 *    - Stepper has 200 steps total
 *    - Microstep modes (MS1, MS2): full (0,0), half (1,0), 1/4 (0,1), 1/8 (1,1)
 *    - Each step = 1.8 degrees, each digit on lock = 9 degrees, so each number = 5 steps
 *    
 *  Hardware Notes:
 *    - 20 grooves around head, 12.8 mm diameter  
 *    
**/


int stepPin = 9;
int dirPin = 10;
int ms1Pin = 7;
int ms2Pin = 6;

bool CW = true; //relative to dial
bool CCW = false; //relative to dial

//stores the position of the dial
int dialPosition = 0;
bool turnDir = CW;

void setup() {               
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(ms1Pin, OUTPUT);
  pinMode(ms2Pin, OUTPUT);

  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  digitalWrite(ms1Pin, LOW);
  digitalWrite(ms2Pin, LOW);
  Serial.begin(9600);

}

String dataEntered = "";
void loop() {
  if (Serial.available() > 0) {
    dataEntered = Serial.readString();
    if (dataEntered == "CW") {
      turnDir = CW;
      Serial.println("Turning CW");
    } else if (dataEntered == "CCW") {
      turnDir = CCW;  
      Serial.println("Turning CCW");
    } else {
      int stepNum = dataEntered.toInt();
      turnTo(stepNum, turnDir);
    }
  }
}

//takes a specified number of steps in either cw or ccw (ccw is positive, cw is negative in relation to dial. reverse for face up testing)
void step(int num, bool dir) {
  if (dir == CCW) {
    //ccw relative to lock
    digitalWrite(dirPin, HIGH);
  } else {
    //cw relative to lock
    digitalWrite(dirPin, LOW);  
  }

  for (int i = 0; i < abs(num); i++) {
    digitalWrite(stepPin, HIGH);
    delay(1);         
    digitalWrite(stepPin, LOW);
    delay(1); 
    
  }
  //Serial.println(num);
}


//takes a specified number of digits to turn in either cw or ccw
void turnDigits(int num, bool dir) {
  step(num*5, dir);
}

//turns to a specified number, uses bool as direction. reverse for face up testing
void turnTo(int dest, bool dir) {
  int turnDistance;
  Serial.print("Current Position:");
  Serial.println(dialPosition);

  //puts destination within 0 - 40 range
  dest = limitInDial(dest);


  if (dir == CW) {
    turnDistance = (40 - dest) + dialPosition;
    turnDistance = limitInDial(turnDistance);
  } else if (dir == CCW) {
    turnDistance = dest - dialPosition;
    turnDistance = limitInDial(turnDistance);
  }

  turnDigits(turnDistance, dir);
  dialPosition = dest;
  
  Serial.print("New Position:");
  Serial.println(dialPosition);
}


//puts an int within 0 <= x <40
int limitInDial(int x) {
  while (x >= 40) {
    x-=40;
  }
  while (x < 0) {
    x+=40;  
  }
  return x;  
}
