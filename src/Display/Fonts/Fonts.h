#pragma once

#include <stdint.h>

struct FONT { 
  const uint8_t* FontTable;
  uint8_t Width;
  uint8_t Height;
  uint8_t AsciiStart;
};