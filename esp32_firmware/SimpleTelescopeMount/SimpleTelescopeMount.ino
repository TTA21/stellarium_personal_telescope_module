#include "NEMAMotor.h"
#include "LinearNEMAMotor.h"
#include "serialHelpers.ino"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

void handleSerialInputN(String axis, float value);
void handleSerialInputS();
void handleSerialInputT(float az, float alt, float frot);
void handleSerialInputP();

typedef struct {
  bool requested;
  double value;
  String axis;
} NudgeCommand;

NudgeCommand nudgeCmd = { false, 0.0, "" };
SemaphoreHandle_t nudgeMutex;

////////////////////////////////////////////////////////////////////////////

NEMAMotor altMotor( 23, 22, 34    ,        //DIR   , PUL   , HOME_PIN
                    20000.0, 20000.0 ,        //SPEED , ACCEL
                    256    , 25.0   , 200,  //MSTEP , REDUC , MOT_TYPE
                    0.001, 0.001  ,        //NBKLSH, PBKLSH 
                    "ALT"             //preferences name
                  );

NEMAMotor fRotMotor( 21, 19,  34    ,        //DIR   , PUL   , HOME_PIN
                     20000.0, 20000.0 ,        //SPEED , ACCEL
                     32    , 99.5   , 200,  //MSTEP , REDUC , MOT_TYPE
                     0.001, 0.001  ,        //NBKLSH, PBKLSH 
                     "FROT"       //preferences name
                   );

AccelStepper fRotMotorTentioner(AccelStepper::FULL4WIRE, 13, 14, 12, 27);
const float stepsPerRev28BYJ = 2048.0;
const float stepsPerDegree28BYJ = stepsPerRev28BYJ / 360.0;

LinearNEMAMotor focuser( 18, 5, 34  ,        //DIR   , PUL   , HOME_PIN
                         1000.0, 500.0,     //SPEED , ACCEL
                         256, 1.0, 200,     //MSTEP , REDUC , MOT_TYPE
                         0.01, 0.01,        //NBKLSH, PBKLSH 
                         2.8, 82.0,         //MMREV , MAXTRAVEL
                         "FOC"
                        );

NEMAMotor azMotor ( 4, 2, 34    ,        //DIR   , PUL   , HOME_PIN
                    20000.0, 20000.0 ,        //SPEED , ACCEL
                    256    , 25.0   , 200,  //MSTEP , REDUC , MOT_TYPE
                    0.001, 0.001  ,        //NBKLSH, PBKLSH 
                    "AZ"              //preferences name
                  );

//////////////////////////////////////////////////////////////////////////////////
#define MOSI_PIN 25
#define MISO_PIN 35  // Input-only is fine for MISO
#define SCLK_PIN 33
#define CS1_PIN 26
#define CS2_PIN 15

////////////////////////////////////////////////////////////////////////////////
// Pin to Core 0 (weak core)
TaskHandle_t SerialTask;

void setup() {
  Serial.begin(115200);


  altMotor.begin();
  azMotor.begin();
  fRotMotor.begin();
  focuser.begin();

  azMotor.setInverted(false);

  fRotMotorTentioner.setMaxSpeed(10);
  fRotMotorTentioner.setSpeed(10);	

  nudgeMutex = xSemaphoreCreateMutex();

  if (nudgeMutex == NULL) {
    Serial.println("Failed to create nudge mutex!");
    while (true); // Fatal error
  }
  
  Serial.println("ESP32 Serial Parser Starting...");
  Serial.println("Commands: N AXIS VALUE | S | T AZ ALT FROT");
  
  // Create serial parser task on Core 0 (weak core)
  xTaskCreatePinnedToCore(
    serialParserTask,   // Function
    "SerialParser",     // Name
    10000,              // Stack size
    NULL,               // Parameters
    1,                  // Priority
    &SerialTask,        // Task handle
    0                   // Core 0 (weak core)
  );

}

bool lastMovementDoneMem = false;

void loop() {

  if (xSemaphoreTake(nudgeMutex, 0)) {  // non-blocking
    if (nudgeCmd.requested) {

      const double val = nudgeCmd.value;
      const String axis = nudgeCmd.axis;

      // Clear command FIRST
      nudgeCmd.requested = false;
      nudgeCmd.value = 0.0;
      nudgeCmd.axis = "";

      xSemaphoreGive(nudgeMutex);

      // Execute on Core 1 (safe, blocking allowed)
      if (axis == "ALT")  altMotor.nudge(val);
      else if (axis == "AZ") azMotor.nudge(val);
      else if (axis == "FROT") fRotMotor.nudge(val);
      else if (axis == "FOC") focuser.setDestination(val);  //Focuser doesnt have a nudge
      else if (axis == "HFOC") focuser.startHoming();  //Reusing the nudge suport to home focuser


      return; // Skip run() this cycle
    }
    xSemaphoreGive(nudgeMutex);
  }

  altMotor.run();
  azMotor.run();
  fRotMotor.run();
  focuser.run();
  
  fRotMotorTentioner.runSpeed();
}

void handleSerialInputN(String axis, float value) {
  if (xSemaphoreTake(nudgeMutex, portMAX_DELAY)) {
    nudgeCmd.requested = true;
    nudgeCmd.value = value;
    nudgeCmd.axis = axis;
    xSemaphoreGive(nudgeMutex);
  }
  // Can't call nudge() here (blocking + wrong core)
}

void handleSerialInputS() {
  const double az = azMotor.getCurrentPositionDeg();
  const double alt = altMotor.getCurrentPositionDeg();
  Serial.print("S ");
  Serial.print(az,5);
  Serial.print(" ");
  Serial.println(alt,5);
  Serial.println();
}

void handleSerialInputT(float az, float alt, float frot) {
  if(!azMotor.isMotorRunning() && !altMotor.isMotorRunning() && !fRotMotor.isMotorRunning()){
    azMotor.setDestination(az);
    altMotor.setDestination(alt);
    fRotMotor.setDestination(frot);
  }
}

void handleSerialInputP(){
  const String altMotParams = altMotor.getParametersCSV();
  const String azMotParams = azMotor.getParametersCSV();
  const String fRotMotParams = fRotMotor.getParametersCSV();
  const String focuserParmas = focuser.getParametersCSV();

  Serial.println( "P ALT," + altMotParams );
  Serial.println( "P AZ," + azMotParams );
  Serial.println( "P FROT," + fRotMotParams );
  Serial.println( "P FOC," + focuserParmas );
  Serial.println();
}


void handleSerialC(char* axis, float speed, float accel, int microstep, 
                   float reduction, int motorSteps, float positiveBacklash, 
                   float negativeBacklash, int homingPin, int inverted){

  if(speed <= 0 || accel <= 0) {
    Serial.println("C ERR Speed or Accel equal or less than 0");
    return;
  }

  if(speed > 20000 || accel > 20000){
    Serial.println("C ERR Speed or Accel too high, > 20000");
    return;
  }

  if( (microstep != 2) && (microstep != 4) && (microstep != 8) && (microstep != 16) && (microstep != 32) && (microstep != 64) && (microstep != 128) && (microstep != 256) ){
    Serial.println("C ERR Microsteps must be 2, 4, 8, 16, 32, 64, 128, 256");
    return;
  }

  if(reduction <= 0){
    Serial.println("C ERR Reduction equal or less than 0");
    return;
  }

  if( (motorSteps != 200) && (motorSteps != 400) ){
    Serial.println("C ERR Motor Natural Steps must be 200 or 400");
    return;
  }

  if( negativeBacklash < 0 || positiveBacklash < 0 ){
    Serial.println("C ERR Backlash must be 0 or a positive number");
    return;
  }

  if(inverted != 0 && inverted != 1){
    Serial.println("C ERR Inverted must be between 0 and 1");
    return;
  }

  if(strcmp(axis, " ALT") == 0){
    altMotor.setSpeed(speed);
    altMotor.setAccel(accel);
    altMotor.setMicrostep(microstep);
    altMotor.setReduction(reduction);
    altMotor.setMotorSteps(motorSteps);
    altMotor.setPositiveBacklash(positiveBacklash);
    altMotor.setNegativeBacklash(negativeBacklash);
    altMotor.setInverted(inverted);
  }

  if(strcmp(axis, " AZ") == 0){
    azMotor.setSpeed(speed);
    azMotor.setAccel(accel);
    azMotor.setMicrostep(microstep);
    azMotor.setReduction(reduction);
    azMotor.setMotorSteps(motorSteps);
    azMotor.setPositiveBacklash(positiveBacklash);
    azMotor.setNegativeBacklash(negativeBacklash);
    azMotor.setInverted(inverted);
  }

  if(strcmp(axis, " FROT") == 0){
    fRotMotor.setSpeed(speed);
    fRotMotor.setAccel(accel);
    fRotMotor.setMicrostep(microstep);
    fRotMotor.setReduction(reduction);
    fRotMotor.setMotorSteps(motorSteps);
    fRotMotor.setInverted(inverted);
  }

  if(strcmp(axis, " FOC") == 0){
    focuser.setSpeed(speed);
    focuser.setAccel(accel);
    focuser.setMicrostep(microstep);
    focuser.setReduction(reduction);
    focuser.setMotorSteps(motorSteps);
    focuser.setPositiveBacklash(positiveBacklash);
    focuser.setNegativeBacklash(negativeBacklash);
  }

  Serial.println("C OK");
}





