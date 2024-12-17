
// ****************************************************************************
//
// Backlight LEDS functions
//
// ****************************************************************************

#include <leds.h>

Adafruit_NeoPixel *neoPixel;

void initLeds(Adafruit_NeoPixel *backlightLeds, uint8_t serial_pin) {
  pinMode(serial_pin, OUTPUT);

  neoPixel = backlightLeds;
  neoPixel->begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  neoPixel->show();            // Turn OFF all pixels ASAP
  neoPixel->setBrightness(15); // Set BRIGHTNESS to about 1/5 (max = 255)
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color) {
  for(int i=0; i<neoPixel->numPixels(); i++) { // For each pixel in strip...
    neoPixel->setPixelColor(i, color);         //  Set pixel's color (in RAM)
    neoPixel->show();                          //  Update strip to match
  }
}

// // Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// // a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// // between frames.
// void theaterChase(uint32_t color, int wait) {
//   for(int a=0; a<10; a++) {  // Repeat 10 times...
//     for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...cap
//       backlightLeds.clear();         //   Set all pixels in RAM to 0 (off)
//       // 'c' counts up from 'b' to end of strip in steps of 3...
//       for(int c=b; c<backlightLeds.numPixels(); c += 3) {
//         backlightLeds.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//       }
//       backlightLeds.show(); // Update strip with new contents
//       delay(wait);  // Pause for a moment
//     }
//   }
// }

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow()
{
  static long firstPixelHue = 0;

  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:

  // strip.rainbow() can take a single argument (first pixel hue) or
  // optionally a few extras: number of rainbow repetitions (default 1),
  // saturation and value (brightness) (both 0-255, similar to the
  // ColorHSV() function, default 255), and a true/false flag for whether
  // to apply gamma correction to provide 'truer' colors (default true).
  neoPixel->rainbow(firstPixelHue);
  // Above line is equivalent to:
  // strip.rainbow(firstPixelHue, 1, 255, 255, true);
  neoPixel->show(); // Update strip with new contents
  // delay(wait);  // Pause for a moment

  firstPixelHue += 256;
  if (firstPixelHue >= 5*65536) firstPixelHue = 0;

}

// // Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
// void theaterChaseRainbow(int wait) {
//   int firstPixelHue = 0;     // First pixel starts at red (hue 0)
//   for(int a=0; a<30; a++) {  // Repeat 30 times...
//     for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
//       backlightLeds.clear();         //   Set all pixels in RAM to 0 (off)
//       // 'c' counts up from 'b' to end of strip in increments of 3...
//       for(int c=b; c<backlightLeds.numPixels(); c += 3) {
//         // hue of pixel 'c' is gOffset by an amount to make one full
//         // revolution of the color wheel (range 65536) along the length
//         // of the strip (strip.numPixels() steps):
//         int      hue   = firstPixelHue + c * 65536L / backlightLeds.numPixels();
//         uint32_t color = backlightLeds.gamma32(backlightLeds.ColorHSV(hue)); // hue -> RGB
//         backlightLeds.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//       }
//       backlightLeds.show();                // Update strip with new contents
//       delay(wait);                 // Pause for a moment
//       firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
//     }
//   }
// }

