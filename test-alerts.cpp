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
  Email::recepient = "";
  Email::setEmail("abc@mail.com");
  REQUIRE(Email::getEmail() == "abc@mail.com");
  REQUIRE(emailTarget.targetInterface(NORMAL) == "");
  REQUIRE(emailTarget.targetInterface(TOO_LOW) == "To: abc@mail.com \n Hi, the temperature is too low");
  TargectSelector controllerTarget(new Controller());
  REQUIRE(controllerTarget.targetInterface(NORMAL) == "");
  REQUIRE(controllerTarget.targetInterface(TOO_HIGH) == "feed : 2");
}