#pragma once

struct PanelConfig {
  bool valid = false;

  bool hasDuration = false;
  unsigned int displayDurationS = 0;

  bool hasBrightness = false;
  unsigned int brightnessPct = 0;
};
