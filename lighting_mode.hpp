#include <FastLED.h>

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
  CRGB::Red,
  CRGB::Gray,  // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};


class Animation {
public:
    Animation(CRGB *leds, size_t leds_count)
    : leds_array(leds)
    , leds_count(leds_count)
    {};

    ~Animation() {};

    void animate() {
        // Get the proper method pointer
        auto the_animation_fn = this->animation_fns[this->current_animation];
        (this->*the_animation_fn)();
    };

    void changeAnimation() {
        this->current_animation += 1;
        this->current_animation %= this->animations_count;
    };

    void changePalette() {
        this->current_palette += 1;
        this->current_palette %= this->palettes_count;
    }

    void setBrightness(uint8_t brightness) {
        this->brightness = brightness;
    };

private:
    // LED array handling (fill from palette)
    CRGB *leds_array;
    size_t leds_count;
    uint8_t brightness = 255;

    void fill_leds_from_palette() {
        uint8_t color_index = this->animation_start_index;
        for (size_t led = 0; led < this->leds_count; led++) {
            leds_array[led] = ColorFromPalette(
                this->palettes[this->current_palette],
                color_index,
                this->brightness,
                this->blend_type
            );
            color_index += 3;
        }
    }


    // Palettes handling
    static const int palettes_count = 2;
    CRGBPalette16 palettes[palettes_count] = {
        RainbowColors_p,
        CRGBPalette16(myRedWhiteBluePalette_p),
    };
    int current_palette;
    TBlendType blend_type = LINEARBLEND;
    uint8_t palette_last_second = 99;

    void periodically_change_palette() {
        uint8_t curr_second = (millis() / 1000) % 60;
        if (palette_last_second != curr_second) {
            palette_last_second = curr_second;
            if (curr_second == 0) {
                this->changePalette();
            }
        }
    }


    // Animation process
    static const int animations_count = 1;
    void (Animation::*animation_fns[animations_count])() = {
        &Animation::simple_animation
    };
    int current_animation = 0;
    int animation_start_index = 0;


    // A simple loop-and-change-palette animation
    void simple_animation() {
        this->animation_start_index += 3;
        // this->periodically_change_palette();
        this->fill_leds_from_palette();
    }
};
