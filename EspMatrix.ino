#include <FS.h>

#include "Display.hpp"
#include "ModBrightness.hpp"
#include "ModControl.hpp"
#include "ModServer.hpp"
#include "ModClock.hpp"
#include "ModWeather.hpp"
#include "ModWifiStatus.hpp"
#include "ModuleIf.hpp"

ModServer modServer{};
ModControl modControl{};
ModBrightness modBrightness{};
ModClock modClock{};
ModWeather modWeather{};
ModWifiStatus modWifiStatus{};

ModuleIf::List allModules{
    &modServer,
    &modControl,
    &modBrightness,
    &modClock,
    &modWeather,
    &modWifiStatus,
};

ModuleIf::List displayModules{
    &modClock,
    &modWeather,
};

ModuleIf *visibleModule{&modClock};

void setup()
{
  Serial.begin(115200);
  SPIFFS.begin();

  Display::setup();

  for (auto module : allModules)
  {
    module->setup();
  }
}

void loop()
{
  for (auto module : allModules)
  {
    module->loop();
  }

  modBrightness.show();

  if (visibleModule)
  {
    visibleModule->show();
  }

  modWifiStatus.show();
  Display::matrix.show();
}
