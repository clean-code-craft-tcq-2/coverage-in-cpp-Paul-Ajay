#include "typewise-alert.h"
#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

std::map<CoolingType, std::vector<int>> temperature_limit_map {
  {PASSIVE_COOLING, {0, 35}},
  {HI_ACTIVE_COOLING, {0, 45}},
  {MED_ACTIVE_COOLING, {0,40}}
};

BreachType classifyTemperatureBreach(
    CoolingType coolingType, double temperatureInC) {
  return inferBreach(temperatureInC, temperature_limit_map[coolingType].at(0), temperature_limit_map[coolingType].at(1));
}

void checkAndAlert(
    AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {

  BreachType breachType = classifyTemperatureBreach(
    batteryChar.coolingType, temperatureInC
  );

  switch(alertTarget) {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}


std::map<BreachType, std::string> breach_message_map = {
  {BreachType::TOO_LOW, "Hi, the temperature is too low"},
  {BreachType::TOO_HIGH, "Hi, the temperature is too high"}
};

void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  if(breachType != NORMAL) {
    std::cout<<"To: "<< recepient<<std::endl;
    std::cout<< breach_message_map[breachType] <<std::endl;
  }
  // switch(breachType) {
  //   case TOO_LOW:
  //     printf("To: %s\n", recepient);
  //     printf("Hi, the temperature is too low\n");
  //     break;
  //   case TOO_HIGH:
  //     printf("To: %s\n", recepient);
  //     printf("Hi, the temperature is too high\n");
  //     break;
  //   case NORMAL:
  //     break;
  // }
}
