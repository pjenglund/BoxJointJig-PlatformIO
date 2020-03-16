
#include <Arduino.h>
#include "BoxJointJig.h"

// CHANGE DEPENDING ON REQUIREMENTS:
const float bladeKerf = 2.8;
const float fingerWidth = 5.0;
const boolean startWithSlot = false;
const boolean DO_LOG = false;

// CONSTANTS FOR GLOBAL USE
const int maxSpeed = 250;
const int mmPerRev = 5.0;
const int stepsPerRev = 1000;
const double expBladeKerf = 0;

// INTERRUPTS
const int interruptHomeSwitch = 2; // used for home switch limit switch, NC (Normally Closed --> HIGH)
const int interruptEndSwitch = 3;  // pin for end limit swith, NC (Normally Closed --> HIGH)

// STEPPER DRIVER PINS
const int pulsePin = 11;
const int directionPin = 12;

// BUTTON PINS
const int homeButton = 8;
const int moveButton = 9;
const int backwardButton = 6;
const int forwardButton = 7;
const int resetButton = 5;

BoxJointJig StepperMotor(pulsePin, directionPin, maxSpeed, mmPerRev, stepsPerRev);

int cutNumber = 0;
int fingerNumber = 0;
long slotFirstPosition = 0;
boolean enableCutting = false;

boolean moveButtonPressed = false;
boolean homeButtonPressed = false;
boolean homeButtonPressState = false;
boolean backwardButtonPressed = false;
boolean forwardButtonPressed = false;

void homeTrigger();
void endTrigger();

void setup()
{
  // NOTE - Interrupt limit switches has to be Normally Closed = HIGH
  attachInterrupt(digitalPinToInterrupt(interruptHomeSwitch), homeTrigger, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptEndSwitch), endTrigger, FALLING);
  pinMode(pulsePin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(homeButton, INPUT);
  pinMode(moveButton, INPUT);
  pinMode(backwardButton, INPUT);
  pinMode(forwardButton, INPUT);
  digitalWrite(directionPin, LOW);

  if (DO_LOG)
  {
    Serial.begin(2000000);

    Serial.println("******************************************************************************");
    Serial.print("Position: ");
    Serial.println(StepperMotor.getPosition());

    Serial.print("BladeKerf: ");
    Serial.println(bladeKerf);

    Serial.print("Finger width: ");
    Serial.println(fingerWidth);

    Serial.print("expBladeKerf: ");
    Serial.println(expBladeKerf);
  }
}

void loop()
{
  // backwardButton button logic----------------------------------------------------------------
  if (digitalRead(backwardButton) == HIGH)
  {
    //Serial.println("backwardButton is HIGH");
    StepperMotor.step(-1);
    enableCutting = false;
    delayMicroseconds(maxSpeed);
  }

  // forwardButton button logic----------------------------------------------------------------
  if (digitalRead(forwardButton) == HIGH)
  {
    //Serial.println("forwardButton is HIGH");
    StepperMotor.step(1);
    enableCutting = false;
    delayMicroseconds(maxSpeed);
  }

  // Home button logic----------------------------------------------------------------
  if (digitalRead(homeButton) == HIGH && homeButtonPressed == false)
  {
    delay(100);
    homeButtonPressed = true;
    StepperMotor.setPosition(0);
    cutNumber = 0;
    fingerNumber = 0;
    slotFirstPosition = 0;
    enableCutting = true;
    // Vibrate to indicate home set
    StepperMotor.stepSteps(5);
    delay(100);
    StepperMotor.stepSteps(-5);
  }
  if (digitalRead(homeButton) == LOW && homeButtonPressed == true)
    homeButtonPressed = false;

  // Move button logic----------------------------------------------------------------
  if (digitalRead(moveButton) == HIGH && moveButtonPressed == false && enableCutting == true)
  {
    delay(100);
    moveButtonPressed = true;
    cutNumber++;
    switch (cutNumber)
    {
    case 1:
      fingerNumber++;
      // DO THIS FOR FIRST CUT ONLY
      if (slotFirstPosition == 0 && startWithSlot == false)
      {
        // Substract half of the expBladeKerf to be just a bit shorter than needed, making for a wider slot and narrower finger
        StepperMotor.stepMm(fingerWidth + bladeKerf - expBladeKerf);
        Serial.println(StepperMotor.stepsToMm(StepperMotor.getPosition()));
      }
      if (slotFirstPosition == 0 && startWithSlot == true)
      {
        StepperMotor.stepMm(bladeKerf);
      }

      // IF WE ARE IN ANY OTHER THAN FIRST CUT, DO THIS
      if (slotFirstPosition != 0)
      {
        StepperMotor.stepSteps((slotFirstPosition - StepperMotor.getPosition()) + StepperMotor.mmToSteps(2 * fingerWidth - expBladeKerf));
        if (DO_LOG)
          Serial.println(StepperMotor.stepsToMm(StepperMotor.getPosition()));
      }

      slotFirstPosition = StepperMotor.getPosition();
      break;
    case 2:
      // MOVE FORWARD TO THE OTHER END OF THE SLOT AND ADD 0.5 expBladeKerf TO MAKE IT SLIGHTLY WIDER
      //StepperMotor.stepMm(fingerWidth + expBladeKerf);
      StepperMotor.stepMm(fingerWidth - bladeKerf + expBladeKerf);
      if (DO_LOG)
        Serial.println(StepperMotor.stepsToMm(StepperMotor.getPosition()));
      // CHECK IF WE HAVE COME FAR ENOUGHT AND CUT ALL THE MATERIAL --> READY TO CUT THE NEXT SLOT
      if (slotFirstPosition > (StepperMotor.getPosition() - StepperMotor.mmToSteps(bladeKerf * 0.9)))
        cutNumber = 0;
      break;
    default:
      StepperMotor.stepMm(-(bladeKerf * 0.9));
      if (DO_LOG)
        Serial.println(StepperMotor.stepsToMm(StepperMotor.getPosition()));
      // CHECK IF WE HAVE COME FAR ENOUGHT AND CUT ALL THE MATERIAL --> READY TO CUT THE NEXT SLOT
      if (slotFirstPosition > (StepperMotor.getPosition() - StepperMotor.mmToSteps(bladeKerf * 0.9)))
        cutNumber = 0;
      break;
    }
  }
  if (digitalRead(moveButton) == LOW && moveButtonPressed == true)
    moveButtonPressed = false;
}

void homeTrigger()
{
  StepperMotor.stepSteps(10);
}

void endTrigger()
{
  StepperMotor.stepSteps(-10);
}
