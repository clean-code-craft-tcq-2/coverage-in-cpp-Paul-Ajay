#include "typewise-alert.h"
#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

std::map<CoolingType, std::vector<int>> temperatureLimitMap {
  {PASSIVE_COOLING, {0, 35}},
  {HI_ACTIVE_COOLING, {0, 45}},
  {MED_ACTIVE_COOLING, {0,40}}
};

BreachType classifyTemperatureBreach(
    CoolingType coolingType, double temperatureInC) {
  return inferBreach(temperatureInC, temperatureLimitMap[coolingType].at(0), temperatureLimitMap[coolingType].at(1));
}

void consolePrint(std::string stringToPrint) {
  std::cout<<stringToPrint<<std::endl;
}

void Controller::sendToController(BreachType breachType, void (*functionPointer)(std::string)) {
  const unsigned short header = 0xfeed;
  std::stringstream outputMessage;
  outputMessage << std::hex << header << " : " << std::hex << breachType;
  (*functionPointer)(outputMessage.str());
}

std::map<BreachType, std::string> breachMessageMap = {
  {BreachType::TOO_LOW, "Hi, the temperature is too low"},
  {BreachType::TOO_HIGH, "Hi, the temperature is too high"}
};

void Email::sendToEmail(BreachType breachType, void (*functionPointer)(std::string)) {
  const char* recepient = "a.b@c.com";
  if(breachType != NORMAL) {
    std::string outputMessage;
    outputMessage = "To: " + std::string(recepient) + "\n" + breachMessageMap[breachType];
    (*functionPointer)(outputMessage);
  }
}

void checkAndAlert(
    TargectSelector targetSelected, BatteryCharacter batteryChar, double temperatureInC, void (*functionPointer)(std::string)) {

  BreachType breachType = classifyTemperatureBreach(
    batteryChar.coolingType, temperatureInC
  );
  targetSelected.targetInterface(breachType, functionPointer);
}
