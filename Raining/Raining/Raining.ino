/* This script is meant to simulate rain drops going down the skirt
 * The idea is to have multiple drops going down at a time
 * Each drop will progress down one of the sides of the skirt at a time
 */

#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>


#define NUM_LEDS 116 //29 leds per strip * 4 strips
#define DATA_PIN 9

#define COLORA CRGB::Fuchsia
#define COLORB CRGB::DarkGoldenrod
#define COLOROFF CRGB::Black
#define WAVESIZE 10
#define NUM_WAVES 4

#define DECAYRATE 0.2

double speedA = 0.029; // the speed for which the lights move

CRGB leds[NUM_LEDS]; // array for each of the LEDs
int brightness_red; // brightness of each color
int brightness_blue;
int brightness_green;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // Initialize the LEDS 
  //StartDark(*leds, NUM_LEDS); // start all the LEDS as dark  
}

void loop() {
  // put your main code here, to run repeatedly:

  int WaveStart[NUM_LEDS] = {0};
  int time_between_waves = (int)((NUM_LEDS / speedA) / NUM_WAVES);

  unsigned long time = millis();

  for (int position = 0; position < NUM_LEDS; position++) {
  leds[position] = COLOROFF;
  }

  for (int wave_number = 0; wave_number < NUM_WAVES; wave_number++) {

  
  if (time > wave_number * time_between_waves) {
  
    WaveStart[wave_number] = int(floor((time - wave_number * time_between_waves) * speedA)) % NUM_LEDS; 
 
    for (int position = WaveStart[wave_number]; (position < (WaveStart[wave_number] + WAVESIZE)) and (position < NUM_LEDS); position++) {
      int shift = (WAVESIZE + WaveStart[wave_number] - position) - 1;
      float s_leftalign = exp(-1 * DECAYRATE * shift);
      brightness_blue = 75 * pow(s_leftalign,2);
      brightness_green = 75 * s_leftalign;
      if (wave_number%2 == 0)
      {
        leds[position].blue += brightness_blue;
        leds[position].green += brightness_green;
      }
      else
      {
        leds[position].red += brightness_blue;
        leds[position].blue += brightness_green;
      }
    }
  }
  }

  // turn on the LEDs
  FastLED.show();
}
