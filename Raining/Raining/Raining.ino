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
#define NUM_WAVES 6

#define DECAYRATE 0.2

double speedA = 0.029*1.5; // the speed for which the lights move

CRGB leds[NUM_LEDS]; // array for each of the LEDs
int brightness_a; // brightness of each color
int brightness_b;
int brightness_c;
int index[NUM_LEDS];


void setup() {

  for (int i = 0; i < NUM_LEDS; i++)
  {
  if (i >= 0 and i < 29) index[i] = -i + (NUM_LEDS/2 - 1);
  else if (i >= 29 and i < 58) index[i] = -i + (NUM_LEDS/2 - 1);
  else if (i >= 58 and i < 87) index[i] = i;
  else if (i >= 87 and i < 116) index[i] = i;
  }
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // Initialize the LEDS 
  //StartDark(*leds, NUM_LEDS); // start all the LEDS as dark  
}

void loop() {
  // put your main code here, to run repeatedly:

  int WaveStart[NUM_WAVES] = {0};
  int time_between_waves = (int)((NUM_LEDS / speedA) / NUM_WAVES) * 0.7;

  unsigned long time = millis();

  for (int position = 0; position < NUM_LEDS; position++) {
  leds[position] = COLOROFF;
  }

  for (int wave_number = 0; wave_number < NUM_WAVES; wave_number++) {

  
  if (time > wave_number * time_between_waves) {
  
    WaveStart[wave_number] = (int(floor((time - wave_number * time_between_waves) * speedA)) % NUM_LEDS) - WAVESIZE; 
 
    for (int position = WaveStart[wave_number]; (position < (WaveStart[wave_number] + WAVESIZE)) and (position < NUM_LEDS/2); position++) {

      if (position >= 0) {
      int shift = (WAVESIZE + WaveStart[wave_number] - position) - 1;
      float s_leftalign = exp(-1 * DECAYRATE * shift);
      brightness_a = 75 * pow(s_leftalign,2);
      brightness_b = 75 * s_leftalign;
      brightness_c = 25 * pow(s_leftalign,4);

      if (position >= 0 and position < NUM_LEDS/4)
      {
        leds[index[position]].blue += brightness_a * (wave_number + 2) * (1.0/double(NUM_WAVES+1));
        leds[index[position]].green += brightness_b;
        leds[index[position+NUM_LEDS/2]].blue += brightness_a * (wave_number + 2) * (1.0/double(NUM_WAVES+1));
        leds[index[position+NUM_LEDS/2]].green += brightness_b;

      }
      else if (position >= NUM_LEDS/4 and position < NUM_LEDS/2)
      {
        leds[index[position]].red += brightness_a;
        leds[index[position]].blue += brightness_b;
        leds[index[position+NUM_LEDS/2]].red += brightness_a;
        leds[index[position+NUM_LEDS/2]].blue += brightness_b;
      }
      }
    }
  }
  }

  // turn on the LEDs
  FastLED.show();
}
