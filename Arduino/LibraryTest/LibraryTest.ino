#include <minimoto.h>

MiniMoto motor0(0xCE);
MiniMoto motor1(0xD0);

void setup()
{
  motor0.drive(50);
  motor1.drive(50);
  delay(1000);
  motor0.stop();
  motor1.stop();
}

void loop()
{
  
}
