#pragma once

typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING
} CoolingType;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH
} BreachType;

BreachType inferBreach(double value, double lowerLimit, double upperLimit);
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC);

typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

class AlertTargetClass {
public:
  virtual void sendOutput(BreachType, void (*functionPointer)(std::string)) = 0;
};
class Controller : public AlertTargetClass {
public:
  virtual void sendOutput(BreachType breachType, void (*functionPointer)(std::string)) {
    sendToController(breachType, *functionPointer);
  }
private:
  void sendToController(BreachType breachType, void (*functionPointer)(std::string));
};
class Email : public AlertTargetClass {
public:
  virtual void sendOutput(BreachType breachType, void (*functionPointer)(std::string)) {
    sendToEmail(breachType, *functionPointer);
  }
private:
  void sendToEmail(BreachType breachType, void (*functionPointer)(std::string));
};
class TargectSelector {
public:
  TargectSelector( AlertTargetClass * const targetObject) :
  targetObject(targetObject) 
  {}
  void targetInterface(BreachType breachType, void (*functionPointer)(std::string)) {
    targetObject->sendOutput(breachType, *functionPointer);
  }
private:
  AlertTargetClass *targetObject;   
};

void checkAndAlert(
  TargectSelector targetSelected, BatteryCharacter batteryChar, double temperatureInC, void (*functionPointer)(std::string));
