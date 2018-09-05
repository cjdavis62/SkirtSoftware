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


#define NUM_LEDS 300
#define DATA_PIN 9

#define COLORA CRGB::Fuchsia
#define COLORB CRGB::DarkGoldenrod
#define COLOROFF CRGB::Black
#define WAVESIZE 20


double speedA = 0.029; // the speed for which the lights move
double speedB = 0.023; 

CRGB leds[NUM_LEDS]; // array for each of the LEDs
int brightness_red; // brightness of each color
int brightness_blue;
int brightness_green;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // Initialize the LEDS 
  //StartDark(*leds, NUM_LEDS); // start all the LEDS as dark
}
void loop() {

  unsigned long time = millis();

  int WaveA_start = int(floor(time * speedA)) % NUM_LEDS; 
  int WaveB_start = NUM_LEDS - int((floor(time * speedB)) + 50) % NUM_LEDS;

  for (int position = 0; position < NUM_LEDS; position++) {
  leds[position] = COLOROFF;
  }
  
  for (int position = WaveA_start; (position < (WaveA_start + WAVESIZE)) and (position < NUM_LEDS); position++) {
  //int brightness = int(floor(abs(sin(speed * time - (position / (NUM_LEDS * 2))))) * 100);
  //float s = sin(time*speed - (float)(position*PI)/(float)WAVESIZE);
  float s = sin(float((position - WaveA_start)*PI)/(float)WAVESIZE);
  brightness_red = 100*pow(s,12);
  brightness_blue = 50 * pow(s,2);
  leds[position].red += brightness_red;
  leds[position].blue += brightness_blue;
  }
  
  for (int position = WaveB_start; (position > (WaveB_start - WAVESIZE)) and (position < NUM_LEDS) and (position >= 0); position--) {
  float s = sin(float((position - WaveB_start)*PI)/(float)WAVESIZE);
  brightness_green = 100 * pow(s,4);
  brightness_blue = 50 * pow(s,12) + 25 * pow(cos(((float)time * PI) / 5000.0),2) * pow(s,2);
  brightness_red = 100 * pow(sin(((float)time * PI) / 5000.0),2) * pow(s,2);
  leds[position].green += brightness_green;
  leds[position].blue += brightness_blue;
  leds[position].red += brightness_red;
  }

  // turn on the LEDs
  FastLED.show();
}

// Set the LEDs to all be dark
/*void StartDark(CRGB leds, int NumberOfLeds) {
for (int i = 0; i < NumberOfLeds; i++)
  leds[i] = COLOROFF;
}*/

