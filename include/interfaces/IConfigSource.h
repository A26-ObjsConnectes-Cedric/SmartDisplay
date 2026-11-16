#pragma once

#include "models/PanelConfig.h"

class IConfigSource {
 public:
  virtual ~IConfigSource() {}

  virtual void begin() = 0;

  virtual bool readConfig(PanelConfig& config) = 0;
};
