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

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
  return inferBreach(temperatureInC, temperatureLimitMap[coolingType].at(0), temperatureLimitMap[coolingType].at(1));
}

void consolePrint(std::string stringToPrint) {
  std::cout<<stringToPrint<<std::endl;
  // countOfPrintCalls++;
}

bool isBreachOccurred(BreachType breachType) {
  return (breachType == TOO_LOW || breachType == TOO_HIGH);
}

std::string Controller::sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  std::stringstream outputMessage;
  outputMessage << std::hex << header << " : " << std::hex << breachType;
  return outputMessage.str();
}

std::map<BreachType, std::string> breachMessageMap = {
  {BreachType::TOO_LOW, "Hi, the temperature is too low"},
  {BreachType::TOO_HIGH, "Hi, the temperature is too high"},
  {BreachType::NORMAL, ""}
};

std::string Email::sendToEmail(BreachType breachType) {
  std::string outputMessage = "To: " + this->recepient + "\n" + breachMessageMap[breachType];
  return outputMessage;
}

std::string Email::recepient = "";

std::string TargectSelector::targetInterface(BreachType breachType) {
  return this->targetObject->sendOutput(breachType);
}

bool validateCoolingType(CoolingType coolingType) {
  return (coolingType == PASSIVE_COOLING || coolingType == MED_ACTIVE_COOLING || coolingType == HI_ACTIVE_COOLING);
}

AlertStatus checkAndAlert(TargectSelector targetSelected, BatteryCharacter batteryChar, double temperatureInC, void (*functionPointer)(std::string)) {
  AlertStatus alertStatus = NONE;
  if(!validateCoolingType(batteryChar.coolingType)) {
    return alertStatus;
  }
  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  if(!isBreachOccurred(breachType)) {
    alertStatus = ALERT_NOT_REQUIRED;
    return alertStatus;
  } 
  std::string outputMessage = targetSelected.targetInterface(breachType);
  (*functionPointer)(outputMessage);
  alertStatus = ALERT_SEND;
  return alertStatus;
}
