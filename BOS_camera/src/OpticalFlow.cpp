#include <SDHCI.h>
#include "OpticalFlow.hpp"

// External SD instance from main.cpp
extern SDClass SD;

// RGB565 -> simple luma
static inline uint8_t y565(uint16_t c) {
  int r = (c >> 11) & 0x1F;
  int g = (c >> 5)  & 0x3F;
  int b =  c        & 0x1F;
  // simple weighted sum, scaled down
  return (uint8_t)((r*3 + g*6 + b*1) >> 1);
}

bool calcOpticalFlowFromRaw(
  const char* bgPath,
  const char* nowPath,
  int W,
  int H,
  int BS,
  int SEARCH,
  FlowVec* outFlow,
  size_t outFlowSize
) {
  File fBg  = SD.open(bgPath, FILE_READ);
  File fNow = SD.open(nowPath, FILE_READ);

  if (!fBg || !fNow) {
    Serial.println("Failed to open RAW files");
    if (fBg)  fBg.close();
    if (fNow) fNow.close();
    return false;
  }

  int bw = W / BS;
  int bh = H / BS;
  size_t needed = (size_t)bw * (size_t)bh;

  if (outFlowSize < needed) {
    Serial.println("outFlow buffer is too small");
    fBg.close();
    fNow.close();
    return false;
  }

  const int STRIPE_H = BS + 2 * SEARCH;  // 8 + 2*2 = 12 lines

  // Each stripe holds STRIPE_H lines of W pixels
  // Fixed size buffer: max 12 lines * 320 pixels
  static uint16_t bgStripe[12 * 320];   // assuming W<=320, STRIPE_H<=12
  static uint16_t nowStripe[12 * 320];

  for (int by = 0; by < bh; ++by) {
    int baseY = by * BS - SEARCH;

    // Load stripe from SD
    for (int sy = 0; sy < STRIPE_H; ++sy) {
      int y = baseY + sy;
      if (y < 0)     y = 0;
      if (y >= H)    y = H - 1;

      uint32_t offset = (uint32_t)y * (uint32_t)W * 2; // 2 bytes per pixel

      fBg.seek(offset);
      fBg.read((uint8_t*)&bgStripe[sy * W], W * 2);

      fNow.seek(offset);
      fNow.read((uint8_t*)&nowStripe[sy * W], W * 2);
    }

    // Process each block in this block-row
    for (int bx = 0; bx < bw; ++bx) {
      int bestDx = 0;
      int bestDy = 0;
      int32_t bestScore = INT32_MAX;

      // search window
      for (int dy = -SEARCH; dy <= SEARCH; ++dy) {
        for (int dx = -SEARCH; dx <= SEARCH; ++dx) {
          int32_t sad = 0;

          for (int yy = 0; yy < BS; ++yy) {
            int y0_global = by * BS + yy;
            int sy0 = (y0_global - baseY);
            if (sy0 < 0 || sy0 >= STRIPE_H) continue;

            for (int xx = 0; xx < BS; ++xx) {
              int x0 = bx * BS + xx;
              int x1 = x0 + dx;

              if (x1 < 0 || x1 >= W) continue;

              int sy1 = sy0 + dy;
              if (sy1 < 0 || sy1 >= STRIPE_H) continue;

              uint16_t c0 = bgStripe[sy0 * W + x0];
              uint16_t c1 = nowStripe[sy1 * W + x1];

              uint8_t g0 = y565(c0);
              uint8_t g1 = y565(c1);

              sad += abs((int)g0 - (int)g1);
            }
          }

          if (sad < bestScore) {
            bestScore = sad;
            bestDx = dx;
            bestDy = dy;
          }
        }
      }

      size_t idx = (size_t)by * (size_t)bw + (size_t)bx;
      outFlow[idx].dx = (int8_t)bestDx;
      outFlow[idx].dy = (int8_t)bestDy;
    }
  }

  fBg.close();
  fNow.close();
  return true;
}
