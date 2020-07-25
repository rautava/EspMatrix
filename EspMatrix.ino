#include "Matrix.h"
#include "ModBrightness.h"
#include "ModDisplay.h"
#include "ModServer.h"
#include "ModTime.h"
#include "ModuleIf.h"

ModServer modServer{};
ModDisplay modDisplay{};
ModBrightness modBrightness{};
ModTime modTime{};

ModuleIf::List allModules{
    &modServer,
    &modDisplay,
    &modBrightness,
    &modTime};

void setup()
{
  Serial.begin(115200);
  Matrix::setup();

  for (auto module : allModules)
  {
    module->setup();
  }

  modDisplay.setTimeModule(&modTime);
}

void loop()
{
  for (auto module : allModules)
  {
    module->loop();
  }
}
