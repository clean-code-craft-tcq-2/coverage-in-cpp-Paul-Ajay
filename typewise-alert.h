#pragma once
#include <string>

typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING,
  INVALID
} CoolingType;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH
} BreachType;

typedef enum {
  ALERT_SEND,
  ALERT_NOT_REQUIRED,
  NONE
}AlertStatus;

BreachType inferBreach(double value, double lowerLimit, double upperLimit);
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC);

typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

class AlertTargetClass {
public:
  virtual std::string sendOutput(BreachType) = 0;
};
class Controller : public AlertTargetClass {
public:
  virtual std::string sendOutput(BreachType breachType) {
    return sendToController(breachType);
  }
private:
  std::string sendToController(BreachType breachType);
};
class Email : public AlertTargetClass {
public:
  virtual std::string sendOutput(BreachType breachType) {
    return sendToEmail(breachType);
  }
  static void setEmail(std::string email) {
    recepient = email;
  }
  static std::string getEmail() {
    return recepient;
  }
  static std::string recepient;
private:
  std::string sendToEmail(BreachType breachType);
};
class TargectSelector {
public:
  TargectSelector( AlertTargetClass * const targetObject) :
  targetObject(targetObject) 
  {}
  std::string targetInterface(BreachType breachType);
private:
  AlertTargetClass *targetObject;   
};

bool validateCoolingType(CoolingType coolingType);

AlertStatus checkAndAlert(
  TargectSelector targetSelected, BatteryCharacter batteryChar, double temperatureInC);

// int countOfPrintCalls = 0;
// void consolePrint(std::string stringToPrint);

bool isBreachOccurred(BreachType breachType);