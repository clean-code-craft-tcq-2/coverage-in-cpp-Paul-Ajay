#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
  REQUIRE(inferBreach(12, 20, 30) == TOO_LOW);
  REQUIRE(inferBreach(40, 20, 30) == TOO_HIGH);
  REQUIRE(inferBreach(25, 20, 30) == NORMAL);
}
TEST_CASE("classify temperature breach according to cooling") {
  REQUIRE(classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, 40) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, -1) == TOO_LOW);
  REQUIRE(classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, 20) == NORMAL);
}

TEST_CASE("test interface class") {
  TargectSelector emailTarget(new Email());
  Email::setEmail("abc@mail.com");
  REQUIRE(emailTarget.targetInterface(TOO_LOW) == "To: abc@mail.com\nHi, the temperature is too low");
  TargectSelector controllerTarget(new Controller());
  REQUIRE(controllerTarget.targetInterface(TOO_HIGH) == "feed : 1");
}

TEST_CASE("test breach check") {
  REQUIRE(isBreachOccurred(NORMAL) == false);
  REQUIRE(isBreachOccurred(TOO_LOW) == true);
}
// TEST_CASE("test email class") {
//   Email emailTarget;
//   Email::setEmail("abc@mail.com");
//   REQUIRE(Email::getEmail() == "abc@mail.com");
//   REQUIRE(emailTarget.sendOutput(TOO_LOW) == "To: abc@mail.com\nHi, the temperature is too low");
//   REQUIRE(emailTarget.sendOutput(TOO_HIGH) == "To: abc@mail.com\nHi, the temperature is too high");
//   REQUIRE(emailTarget.sendOutput(NORMAL) == ""); 
// }

// TEST_CASE("test controller class") {
//   Controller controllerTarget;
//   REQUIRE(controllerTarget.sendOutput(NORMAL) == "feed : 0");
//   REQUIRE(controllerTarget.sendOutput(TOO_HIGH) == "feed : 2");
//   REQUIRE(controllerTarget.sendOutput(TOO_LOW) == "feed : 1");  
// }

TEST_CASE("test cooling type validation functionality") {
  REQUIRE(validateCoolingType(HI_ACTIVE_COOLING) == true);
  REQUIRE(validateCoolingType(MED_ACTIVE_COOLING) == true);
  REQUIRE(validateCoolingType(PASSIVE_COOLING) == true);
  REQUIRE(validateCoolingType(INVALID) == false);
}
TEST_CASE("test temperature check and alert functionality") {
  TargectSelector controllerTarget(new Controller());
  BatteryCharacter battery;
  battery.coolingType = PASSIVE_COOLING;
  REQUIRE(checkAndAlert(controllerTarget, battery, 40, *consolePrint) == ALERT_SEND);
  TargectSelector emailTarget(new Email());
  battery.coolingType = INVALID;
  REQUIRE(checkAndAlert(emailTarget, battery, 40, *consolePrint) == NONE);
  battery.coolingType = MED_ACTIVE_COOLING;
  REQUIRE(checkAndAlert(emailTarget, battery, 10, *consolePrint) == ALERT_NOT_REQUIRED);
}
