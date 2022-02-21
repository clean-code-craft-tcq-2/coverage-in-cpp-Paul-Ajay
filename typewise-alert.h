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
  virtual void sendOutput(BreachType, void (*fp)(std::string)) = 0;
};
class Controller : public AlertTargetClass {
public:
  virtual void sendOutput(BreachType breachType, void (*fp)(std::string)) {
    sendToController(breachType, fp);
  }
private:
  void sendToController(BreachType breachType, void (*fp)(std::string));
};
class Email : public AlertTargetClass {
public:
  virtual void sendOutput(BreachType breachType, void (*fp)(std::string)) {
    sendToEmail(breachType, fp);
  }
private:
  void sendToEmail(BreachType breachType, void (*fp)(std::string));
};
class TargectSelector {
public:
  TargectSelector( AlertTargetClass * const targetObject) :
  targetObject(targetObject) 
  {}
  void targetInterface(BreachType breachType, void (*fp)(std::string)) {
    targetObject->sendOutput(breachType, fp);
  }
private:
  AlertTargetClass *targetObject;   
};

void checkAndAlert(
  TargectSelector targetSelected, BatteryCharacter batteryChar, double temperatureInC, void (*fp)(std::string));

void sendToController(BreachType breachType, void (*fp)(std::string));
void sendToEmail(BreachType breachType, void (*fp)(std::string));
