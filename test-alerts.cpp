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
}

TEST_CASE("test interface class") {
  TargectSelector emailTarget(new Email());
  Email::setEmail("abc@mail.com");
  REQUIRE(Email::getEmail() == "abc@mail.com");
  REQUIRE(emailTarget.targetInterface(NORMAL) == "");
  REQUIRE(emailTarget.targetInterface(TOO_LOW) == "To: abc@mail.com\nHi, the temperature is too low");
  REQUIRE(emailTarget.targetInterface(TOO_HIGH) == "To: abc@mail.com\nHi, the temperature is too high");
  TargectSelector controllerTarget(new Controller());
  REQUIRE(controllerTarget.targetInterface(NORMAL) == "feed : 0");
  REQUIRE(controllerTarget.targetInterface(TOO_HIGH) == "feed : 2");
  REQUIRE(controllerTarget.targetInterface(TOO_LOW) == "feed : 1");
}

TEST_CASE("test temperature check and alert functionality") {
  TargectSelector controllerTarget(new Controller());
  BatteryCharacter battery;
  battery.coolingType = PASSIVE_COOLING;
  checkAndAlert(controllerTarget, battery, 40, *consolePrint);
}
