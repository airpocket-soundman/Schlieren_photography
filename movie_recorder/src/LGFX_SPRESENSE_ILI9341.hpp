#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// Spresense + ILI9341 (SPI4) 用設定

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9341 _panel_instance;
  lgfx::Bus_SPI       _bus_instance;

public:
  LGFX(void) {
    { // バス設定
      auto cfg = _bus_instance.config();

      cfg.spi_mode   = 0;          // ILI9341 は MODE0
      cfg.freq_write = 40000000;   // 40MHz
      cfg.freq_read  = 16000000;
      cfg.pin_dc     = 9;          // D9 = DC
      cfg.spi_port   = 4;          // SPI4

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    { // パネル設定
      auto cfg = _panel_instance.config();

      cfg.pin_cs   = 10;   // D10 = CS
      cfg.pin_rst  = 8;    // D8  = RESET
      cfg.pin_busy = -1;

      cfg.panel_width     = 240;
      cfg.panel_height    = 320;
      cfg.offset_x        = 0;
      cfg.offset_y        = 0;
      cfg.offset_rotation = 0;
      cfg.readable        = true;
      cfg.invert          = false;
      cfg.rgb_order       = false;
      cfg.dlen_16bit      = false;
      cfg.bus_shared      = true;  // SDカードと共有する場合

      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance);
  }
};
