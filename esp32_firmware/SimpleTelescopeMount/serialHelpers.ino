#ifndef SERIAL_HELPERS_INO
#define SERIAL_HELPERS_INO

void parseCommand(char* cmd);
void parseNudgeCommand(char* params);
void parseTrackingCommand(char* params);
void parseConfiguration(char* params);


// Buffer for incoming serial data
#define BUFFER_SIZE 64
char serialBuffer[BUFFER_SIZE];
uint8_t bufferIndex = 0;

// Function prototypes for your handlers
void handleSerialInputN(String axis, float value);
void handleSerialInputS();
void handleSerialInputT(float az, float alt, float frot);
void printSensorData();
void handleSerialInputP();
void parseConfiguration(char* params);
void handleSerialC(char* axis, float speed, float accel, int microstep, 
                   float reduction, int motorSteps, float positiveBacklash, 
                   float negativeBacklash, int homingPin, int inverted);

// Serial parser task running on Core 0
void serialParserTask(void * parameter) {
  for(;;) {

    while (Serial.available() > 0) {
      char inChar = Serial.read();
      
      // Handle newline as command terminator
      if (inChar == '\n' || inChar == '\r') {
        if (bufferIndex > 0) {
          serialBuffer[bufferIndex] = '\0';
          parseCommand(serialBuffer);
          bufferIndex = 0;
        }
      }
      // Add character to buffer
      else if (bufferIndex < BUFFER_SIZE - 1) {
        serialBuffer[bufferIndex++] = inChar;
      }
      // Buffer overflow protection
      else {
        Serial.println("ERROR: Buffer overflow");
        bufferIndex = 0;
      }
    }
    vTaskDelay(1); // Small delay to prevent watchdog issues
  }
}

void parseCommand(char* cmd) {
  // Skip leading whitespace
  while (*cmd == ' ') cmd++;
  
  if (cmd[0] == '\0') return; // Empty command
  
  char command = cmd[0];
  
  switch(command) {
    case 'N':
    case 'n':
      parseNudgeCommand(cmd + 1);
      break;
      
    case 'S':
    case 's':
      handleSerialInputS();
      break;
      
    case 'T':
    case 't':
      parseTrackingCommand(cmd + 1);
      break;

    case 'P':
    case 'p':
      handleSerialInputP();
      break;

    case 'C':
    case 'c':
      parseConfiguration(cmd + 1);
      break;
      
    default:
      Serial.print("ERROR: Unknown command '");
      Serial.print(command);
      Serial.println("'");
      break;
  }
}

void parseNudgeCommand(char* params) {
  char axis[8];
  float value;
  
  // Parse: "AXIS VALUE" format
  int parsed = sscanf(params, "%s %f", axis, &value);
  
  if (parsed != 2) {
    Serial.println("ERROR: Invalid N command format. Use: N AXIS VALUE");
    return;
  }
  
  // Convert axis to uppercase for comparison
  String axisStr = String(axis);
  axisStr.toUpperCase();
  
  handleSerialInputN(axisStr, value);
}

void parseTrackingCommand(char* params) {
  float az, alt, frot;
  
  // Parse: "AZ ALT FROT" format
  int parsed = sscanf(params, "%f %f %f", &az, &alt, &frot);
  
  if (parsed != 3) {
    Serial.println("ERROR: Invalid T command format. Use: T AZ ALT FROT");
    return;
  }
  
  // Validate ranges
  if (az < 0.0 || az > 360.0) {
    Serial.println("ERROR: AZ must be between 0 and 360");
    return;
  }
  if (alt < -90.0 || alt > 90.0) {
    Serial.println("ERROR: ALT must be between -90 and 90");
    return;
  }
  if (frot < 0.0 || frot > 360.0) {
    Serial.println("ERROR: FROT must be between 0 and 360");
    return;
  }
  
  handleSerialInputT(az, alt, frot);
}

void parseConfiguration(char* params){
  char* token;
  int fieldIndex = 0;
  
  // Variables to store parsed values
  char axis[10];
  float speed = 0.0;
  float accel = 0.0;
  int microstep = 0;
  float reduction = 0.0;
  int motorSteps = 0;
  float positiveBacklash = 0.0;
  float negativeBacklash = 0.0;
  int homingPin = 0;
  int inverted = 0;
  
  // Parse the CSV string
  token = strtok(params, ",");
  
  while(token != NULL && fieldIndex < 10) {
    switch(fieldIndex) {
      case 0: strcpy(axis, token); break;
      case 1: speed = atof(token); break;
      case 2: accel = atof(token); break;
      case 3: microstep = atoi(token); break;
      case 4: reduction = atof(token); break;
      case 5: motorSteps = atoi(token); break;
      case 6: positiveBacklash = atof(token); break;
      case 7: negativeBacklash = atof(token); break;
      case 8: homingPin = atoi(token); break;
      case 9: inverted = atoi(token); break;
    }
    fieldIndex++;
    token = strtok(NULL, ",");
  }
  
  // Call the handler function with parsed parameters
  handleSerialC(axis, speed, accel, microstep, reduction, motorSteps, 
                positiveBacklash, negativeBacklash, homingPin, inverted);
}
#endif