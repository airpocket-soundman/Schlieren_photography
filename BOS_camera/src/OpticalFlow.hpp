#pragma once
#include <Arduino.h>

// Flow vector per block
struct FlowVec {
  int8_t dx;
  int8_t dy;
};

// Compute optical flow from two RAW RGB565 files on SD
// bgPath  : "background.raw"
// nowPath : "current.raw"
// width   : 960
// height  : 720
// BS      : block size (8)
// SEARCH  : search radius (2)
bool calcOpticalFlowFromRaw(
  const char* bgPath,
  const char* nowPath,
  int width,
  int height,
  int BS,
  int SEARCH,
  FlowVec* outFlow,  // size: (width/BS)*(height/BS)
  size_t outFlowSize
);
