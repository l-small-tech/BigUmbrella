#ifndef BIGUMBRELLA_H
#define BIGUMBRELLA_H
#endif

#include <FastLED.h>

// FastLED Settings
#define FLED_DATA_PIN       13
#define FLED_LED_COUNT      400
#define FLED_LED_TYPE       WS2812B
#define FLED_COLOR_ORDER    GRB

// Umbrella Settings
#define UMBRELLA_RIB_COUNT  8
#define UMBRELLA_RIB_LENGTH 50

// Global Animation Settings
#define FPS                 60
#define FADE_AMOUNT         40


// Public class
class BigUmbrella {
  public:
    CRGB* leds = new CRGB[FLED_LED_COUNT];

    // Animation Variables
    // Global
    int index = 0;
    int hue = 0;
    CHSV color = CHSV(hue, 255, 255);
    CHSV ring[UMBRELLA_RIB_COUNT];

    BigUmbrella() {
      // Setup FastLED
      FastLED.addLeds<WS2812B, FLED_DATA_PIN, GRB>(leds, FLED_LED_COUNT).setCorrection( Typical8mmPixel  );
      FastLED.setBrightness(255);

      clear();

      // Initialize ring array
      for (int i=0; i<UMBRELLA_RIB_COUNT; i++) { ring[i] = CHSV(0,0,0); }
    };

    void rainbow() {
      // FastLED's built-in rainbow generator
      fill_rainbow( leds, FLED_LED_COUNT, index, 1);

      index = index + 1 % 255;

      FastLED.show();
      delay(100);
    }

    /* A bit about pixel mapping...

        * Ribs are the X axis, Index along the rib is your Y axis.
        * ssuming 50 pixels per rib, 2 sets.
        * If you divid 25 pixel segments into 16, even numbers are assending and odd numbers
          are decending.
        

        leds[0]   is (0,0)
        leds[1]   is (0,2)
        leds[2]   is (0,4)
        ...
        leds[24]  is (0,48)
        leds[25]  is (0,49)
        leds[26]  is (0,47)
        leds[27]  is (0,45)
        ...
        leds[47]  is (0,5)
        leds[48]  is (0,3)
        leds[49]  is (0,1)
        leds[50]  is (1,0)
    */
    int mapXY(int x, int y) {

      // If Y is even
      if (y % 2 == 0) {
        return y / 2 + x * UMBRELLA_RIB_LENGTH;

      // If Y is odd
      } else {
        return ((x + 1) * UMBRELLA_RIB_LENGTH) - 1 - ((y - 1) / 2);
      }
    }

    void clear() {
      for (int i = 0; i < FLED_LED_COUNT; i++) {
        leds[i] = CRGB::Black;
      }
    }

    int test = 0;
    void experimental() {
      if (test == 0) {
        clear();
        leds[mapXY(1, 0)] = CRGB::White;
        test = 1;
      }



      FastLED.show();
      delay(1000);
      shiftAllY();
    }
    

    void rain() {
      // Pick a random rib to spawn a rain drop on
      int rib = random(0, 9);
      color = CHSV(hue + random(0, 5) % 255, 255 - random(0, 150), 255);
      if (rib < 8) { ring[rib] = color; }

      // Increment the hue
      if (index > 1) {
        hue = hue + 1 % 255;
        index = 0;
      } else { index += 1; }
      
      // for each rib in the ring
      for (int i=0; i<UMBRELLA_RIB_COUNT; i++) {

        // Write the ring to the strip
        leds[mapXY(i, 0)] = ring[i];

        // Fade the colors
        int value = ring[i].v;
        value -= FADE_AMOUNT;
        if (value < 0) { value = 0; }
        ring[i] = CHSV(ring[i].h, ring[i].s, value);
      }
      shiftAllY();
      FastLED.show();
      delay(40);
    }

    void shiftAllY() {
      // Shift all pixels on ribs down by one

      for (int x = 0; x < UMBRELLA_RIB_COUNT; x++) {
        for (int y = UMBRELLA_RIB_LENGTH; y > 0; y--) {
          leds[mapXY(x, y)] = leds[mapXY(x, y - 1)];
        }
      }

      // Clear the top row
      for (int x = 0; x < UMBRELLA_RIB_LENGTH; x++) {
        leds[mapXY(x, 0)] = CRGB::Black;
      }
    }

    void tick() {
      rain();
      // experimental();
      // chaseEffect();
      // rainbow();
      // meteor();
      // delay(100);
      // FastLED.show();
      // solidColour();
      // fill_gradient(leds, FLED_LED_COUNT, CHSV(100,255,255), ), FORWARD_HUES);
      // fill_solid(leds, FLED_LED_COUNT, CRGB::HotPink); 
      // FastLED.show();
      // delay(100);
    }
};
