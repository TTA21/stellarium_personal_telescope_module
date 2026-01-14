#include <AccelStepper.h>
#include <Preferences.h>

class NEMAMotor {
private:
    AccelStepper stepper;
    Preferences prefs;
    String prefsNamespace;
    
    // Motor parameters
    uint8_t dirPin;
    uint8_t pulPin;
    uint8_t homingPin;
    float speed;
    float accel;
    uint16_t microstep;
    float reduction;
    uint16_t motorSteps; // 200 or 400
    double positiveBacklash;
    double negativeBacklash;
    bool inverted;
    
    // Homing state
    bool isHoming;
    
    // Tracking
    double accumulatedFractionalSteps;
    int8_t lastDirection; // 1 for positive, -1 for negative, 0 for not set
    
public:
    NEMAMotor(uint8_t dir, uint8_t pul, uint8_t homingPin, float spd, float acc, 
              uint16_t micro, float red, uint16_t motorType,
              double posBacklash, double negBacklash, String prefsName)
        : stepper(AccelStepper::DRIVER, pul, dir),
          dirPin(dir), pulPin(pul), homingPin(homingPin), speed(spd), accel(acc),
          microstep(micro), reduction(red), motorSteps(motorType),
          positiveBacklash(posBacklash), negativeBacklash(negBacklash),
          prefsNamespace(prefsName), inverted(true), isHoming(false),
          accumulatedFractionalSteps(0.0), lastDirection(0) {
        
        pinMode(homingPin, INPUT);
        
        stepper.setMaxSpeed(speed);
        stepper.setAcceleration(accel);
        
        // Load parameters from NVS
        loadFromMemory();
    }
    
    void begin() {
        prefs.begin(prefsNamespace.c_str(), false);
        prefs.end();
    }
    
    // Setters with NVS storage
    void setSpeed(float spd) {
        speed = spd;
        stepper.setMaxSpeed(speed);
        prefs.begin(prefsNamespace.c_str(), false);
        prefs.putFloat("speed", speed);
        prefs.end();
    }
    
    void setAccel(float acc) {
        accel = acc;
        stepper.setAcceleration(accel);
        prefs.begin(prefsNamespace.c_str(), false);
        prefs.putFloat("accel", accel);
        prefs.end();
    }
    
    void setMicrostep(uint16_t micro) {
        microstep = micro;
        prefs.begin(prefsNamespace.c_str(), false);
        prefs.putUShort("microstep", microstep);
        prefs.end();
    }
    
    void setReduction(float red) {
        reduction = red;
        prefs.begin(prefsNamespace.c_str(), false);
        prefs.putFloat("reduction", reduction);
        prefs.end();
    }
    
    void setMotorSteps(uint16_t steps) {
        motorSteps = steps;
        prefs.begin(prefsNamespace.c_str(), false);
        prefs.putUShort("motorSteps", motorSteps);
        prefs.end();
    }
    
    void setPositiveBacklash(double backlash) {
        positiveBacklash = backlash;
        prefs.begin(prefsNamespace.c_str(), false);
        prefs.putDouble("posBacklash", positiveBacklash);
        prefs.end();
    }
    
    void setNegativeBacklash(double backlash) {
        negativeBacklash = backlash;
        prefs.begin(prefsNamespace.c_str(), false);
        prefs.putDouble("negBacklash", negativeBacklash);
        prefs.end();
    }
    
    void setInverted(bool inv) {
        inverted = inv;
        stepper.setPinsInverted(inverted, false, false);
        prefs.begin(prefsNamespace.c_str(), false);
        prefs.putBool("inverted", inverted);
        prefs.end();
    }
    
    // Getters
    float getSpeed() { return speed; }
    float getAccel() { return accel; }
    uint16_t getMicrostep() { return microstep; }
    float getReduction() { return reduction; }
    uint16_t getMotorSteps() { return motorSteps; }
    double getPositiveBacklash() { return positiveBacklash; }
    double getNegativeBacklash() { return negativeBacklash; }
    uint8_t getHomingPin() { return homingPin; }
    bool getInverted() { return inverted; }
    
    // Get all parameters as CSV
    String getParametersCSV() {
        return String(speed, 6) + "," + 
               String(accel, 6) + "," + 
               String(microstep) + "," + 
               String(reduction, 6) + "," + 
               String(motorSteps) + "," + 
               String(positiveBacklash, 9) + "," + 
               String(negativeBacklash, 9) + "," +
               String(homingPin) + "," +
               String(inverted);
    }

    // Load parameters from NVS memory
    void loadFromMemory() {
        prefs.begin(prefsNamespace.c_str(), true);
        speed = prefs.getFloat("speed", speed);
        accel = prefs.getFloat("accel", accel);
        microstep = prefs.getUShort("microstep", microstep);
        reduction = prefs.getFloat("reduction", reduction);
        motorSteps = prefs.getUShort("motorSteps", motorSteps);
        positiveBacklash = prefs.getDouble("posBacklash", positiveBacklash);
        negativeBacklash = prefs.getDouble("negBacklash", negativeBacklash);
        inverted = prefs.getBool("inverted", inverted);
        prefs.end();
        
        stepper.setMaxSpeed(speed);
        stepper.setAcceleration(accel);
        stepper.setPinsInverted(inverted, false, false);
    }
    
    // Set destination in degrees (-360 to +360)
    void setDestination(double degrees) {
        // Allow -360 to +360 range
        if (degrees < -360.0 || degrees > 360.0) {
            while (degrees < -360.0) degrees += 360.0;
            while (degrees > 360.0) degrees -= 360.0;
        }
        
        // Calculate steps: (degrees / 360) * motorSteps * microstep * reduction
        double exactSteps = (degrees / 360.0) * motorSteps * microstep * reduction;
        
        // Add accumulated fractional steps
        exactSteps += accumulatedFractionalSteps;
        
        // Extract whole steps
        long wholeSteps = (long)exactSteps;
        
        // Store fractional remainder
        accumulatedFractionalSteps = exactSteps - wholeSteps;
        
        // Determine direction and apply backlash compensation
        int8_t newDirection = (wholeSteps > stepper.currentPosition()) ? 1 : -1;
        
        /*if (lastDirection != 0 && lastDirection != newDirection) {
            // Direction change detected, apply backlash
            double backlash = (newDirection > 0) ? positiveBacklash : negativeBacklash;
            long backlashSteps = (long)((backlash / 360.0) * motorSteps * microstep * reduction);
            wholeSteps += backlashSteps * newDirection;
            Serial.print("Direction changed, new direction index : ");
            if(newDirection > 0) Serial.print(" positiveBacklash ");
            else Serial.print(" negativeBacklash ");
            Serial.print(", change of ");
            Serial.print(backlash, 5);
            Serial.println(" degrees.");
        }*/
        
        lastDirection = newDirection;
        stepper.moveTo(wholeSteps);
    }
    
    // Blocking homing - moves backwards until homing pin goes HIGH
    void startHoming() {
        stepper.moveTo(-999999999L);
        
        while (digitalRead(homingPin) == LOW) {
            stepper.run();
        }
        
        stepper.stop();
        stepper.setCurrentPosition(0);
        resetAccumulator();
        lastDirection = 0;
    }
    
    // Run the motor (call this in loop)
    void run() {
        stepper.run();
    }
    
    // Check if motor is running
    bool isMotorRunning() {
        return stepper.isRunning() || isHoming;
    }
    
    // Check if currently homing
    bool isHomingActive() {
        return isHoming;
    }
    
    // Stop homing manually
    void stopHoming() {
        if (isHoming) {
            stepper.stop();
            isHoming = false;
        }
    }
    
    // Reset accumulated fractional steps
    void resetAccumulator() {
        accumulatedFractionalSteps = 0.0;
    }
    
    // Get current position in steps
    long getCurrentPosition() {
        return stepper.currentPosition();
    }
    
    // Set current position as zero
    void setZero() {
        stepper.setCurrentPosition(0);
        resetAccumulator();
        lastDirection = 0;
    }
    
    void nudge(double offsetDegrees) {
      // Save logical position
      long savedPosition = stepper.currentPosition();

      // Calculate steps for the offset
      double exactSteps = (offsetDegrees / 360.0) * motorSteps * microstep * reduction;

      exactSteps += accumulatedFractionalSteps;

      long wholeSteps = (long)exactSteps;
      accumulatedFractionalSteps = exactSteps - wholeSteps;

      if (wholeSteps == 0) return;

      long newTarget = savedPosition + wholeSteps;

      // Direction & backlash handling
      int8_t newDirection = (wholeSteps > 0) ? 1 : -1;
      /*if (lastDirection != 0 && lastDirection != newDirection) {
          double backlash = (newDirection > 0) ? positiveBacklash : negativeBacklash;
          long backlashSteps =
              (long)((backlash / 360.0) * motorSteps * microstep * reduction);
          newTarget += backlashSteps * newDirection;
          Serial.print("NDirection changed, new direction index : ");
          if(newDirection > 0) Serial.print(" positiveBacklash ");
          else Serial.print(" negativeBacklash ");
          Serial.print(", change of ");
          Serial.print(backlash, 5);
          Serial.println(" degrees.");
      }*/
      lastDirection = newDirection;

      stepper.moveTo(newTarget);

      while (stepper.isRunning()) {
          stepper.run();
      }

      stepper.setCurrentPosition(savedPosition);
  }


    double getCurrentPositionDeg(){
      return (360 * stepper.currentPosition()) / (motorSteps * microstep * reduction);
    }
};