// C++ code
#include <IRremote.h>

// Constants  
const int RECV_PIN = 10;
const float INITIAL_KP = 0;
const float INITIAL_KD = 0;

// Estados da RACE 
enum RaceStates {
  WAITING_FOR_CATEGORY,
  WAITING_FOR_CALIBRATION,
  CALIBRATION,
  WAITING_FOR_RACE,
  RACE,
  COMPLETED
};

// Variables
RaceStates currentRaceState = WAITING_FOR_CATEGORY;
bool follower;
// Definition of PID parameters
float kP = INITIAL_KP;
float kD = INITIAL_KD;

const unsigned long ZERO = 4077698816;
const unsigned long ONE = 4010852096;
const unsigned long TWO = 3994140416;
const unsigned long THREE = 3977428736;
const unsigned long NINE = 3843735296;
const unsigned long LEFT_ARROW = 4211392256;
const unsigned long RIGHT_ARROW = 4177968896;
const unsigned long UP_ARROW = 4111122176;
const unsigned long DOWN_ARROW = 4144545536;

void handleRace(unsigned long code)
{
  if(code == THREE)
  {
  	currentRaceState = RACE;
  }
}

void handleCalibration(unsigned long code)
{
	if (code == ONE){
      		// Press 1, use existing calibration
      		Serial.println("Using existing calibration...");
      		currentRaceState = WAITING_FOR_RACE;
    	}else if (code == TWO)
    	{
      		// Press 2 to start calibrating
      		Serial.println("calibrating...");
      		currentRaceState = CALIBRATION;
    	}
}

void handleCategory(unsigned long code)
{
	if (code == NINE)
    {
      // Press 9, change category to follower;
      Seguidor = true;
      currentRaceState = WAITING_FOR_CALIBRATION;
      Serial.println("Follower mode.");
    } else if(code == ZERO)
    {
      // Press 0, change category to stalker;
      Seguidor = false;
      currentRaceState = COMPLETED;
      Serial.println("Stalker Mode.");
    }
}

void adjustPIDParameters(unsigned long code)
{
	 if(code == LEFT_ARROW)
     {
          	// decreases value of kD
            kD -=0.05;
            Serial.print("Valor de kD: ");
            Serial.println(kD);
     }
     if(code == RIGHT_ARROW)
     {
          	// increases value of kD
            kD += 0.05;
            Serial.print("Valor de kD: ");
            Serial.println(kD);
     }
     if(code == UP_ARROW)
     {
          	// increases value of kP
            kP += 0.05;
            Serial.print("Valor de kP: ");
            Serial.println(kP);
     }
     if(code == DOWN_ARROW)
     {
          	// decreases value of kP
            kP -= 0.05;
            Serial.print("Valor de kP: ");
            Serial.println(kP);
     }
}

void readIr() {
  if (IrReceiver.decode()){
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    //Serial.println(code, DEC); // print code received by ir sensor
    IrReceiver.resume(); // For next value
    
    // Section responsible for choosing category
    if (currentRaceState == WAITING_FOR_CATEGORY)
    {
    	handleCategory(code);
    }
    
    // Section for race
    if (Seguidor)
    {
    	if (currentRaceState == WAITING_FOR_CALIBRATION){
      		handleCalibration(code);
        }
    	if (currentRaceState == WAITING_FOR_RACE)
    	{
      		handleRace(code);
    	}  
      // Change PID parameters
      if (currentRaceState == WAITING_FOR_RACE || currentRaceState == RACE)
      {
        adjustPIDParameters(code);
    	}
     }
  }
}


void stateMachine() {
  switch(currentRaceState){
    case (WAITING_FOR_CATEGORY):
      digitalWrite(7, HIGH);
      break;
    case (WAITING_FOR_CALIBRATION):
      break;
    case (CALIBRATION):
      digitalWrite(7, LOW);
      digitalWrite(6, HIGH);
      delay(2000);
      currentRaceState = WAITING_FOR_RACE;
      break; 
    case(WAITING_FOR_RACE):
      break;
    case (RACE):
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);
      Serial.println("Running...");
      delay(3000);
      currentRaceState = COMPLETED;
      break;
    case (COMPLETE):
      digitalWrite(5, LOW);
      Serial.println("COMPLETE.");
      currentRaceState = WAITING_FOR_CATEGORY;
      break;
    default: Serial.println("Choose a category."); break;
  }
  
  switch(currentRaceState){
  case (WAITING_FOR_CATEGORY):
    Serial.println("WAITING_FOR_CATEGORY");
    break;
  case WAITING_FOR_CALIBRATION:
    Serial.println("WAITING_FOR_CALIBRATION");
    break;
  case CALIBRATION: 
    Serial.println("CALIBRATION");
    break;
  case WAITING_FOR_RACE: 
    Serial.println("WAITING_FOR_RACE");
    break;
  case RACE: 
    Serial.println("RACE");
    break;
  case COMPLETED: 
    Serial.println("COMPLETE");
    break;
  }
  
}

void setup()
{
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop()
{
 readIr();
 stateMachine();
 delay(2000);
}
