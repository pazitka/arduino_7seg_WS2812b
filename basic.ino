/**
 7 segment WS2812B
 single segment for testing.

 This is an old project that I had made long time ago.
 I just dont have source code any more. So I'll make it again.

 Originally it was bigger 7segment display to display time and temperature.
 I'm just not sure do I remember how to programm it again.

 @author Michal Kowalewski
 @license BSD-3 use as you want and be happy.
 @git https://github.com/pazitka/arduino_7seg_WS2812b
 @project simulation: https://wokwi.com/projects/384990723567618049

*/

/**
 Sample usage to display 2. on left segment:
 100,100,0,12,2

 Sample usage to display 4 on right segment:
 100,100,0,4,1

 usage schema:
 r,g,b,ledNo,segNo
*/

#include <PololuLedStrip.h>

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<2> ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 16 //number of LED stips and also number of rows in vector
rgb_color colors[LED_COUNT]; //color vector

rgb_color black; //black color
rgb_color color; //color

uint8_t ledNo; //number to be displayed on LEDs
uint8_t segNo; //number 7 segment display
uint8_t diff;  //difference between LED ids

void setup() {
  // put your setup code here, to run once:
  //pinMode(ledInPin,OUTPUT)

  // Start up the serial port, for communication with the PC.
  Serial.begin(115200);
  Serial.println("Ready to receive colors.");

  //black
  black.red = 0;
  black.green = 0;
  black.blue = 0;
}

/**
 Chosing seven display
 @param uint8_t dspNo number of display from 0-n
 */
void d7seg_Seg_Chose(uint8_t dspNo=0)
{
  diff = dspNo*8;
}

/**
 7 Segment show dot
 @param bool state set true if you want dot or false if you don't
 */
void d7seg_dot(bool state)
{
  if(state){
    colors[diff+7] = color;
  }else{
    colors[diff+7] = black;
  }
}

/**
 7 Segment display clear
 */
void d7seg_clear()
{
  for(uint8_t i=0; i<8; i++)
  {
    colors[diff+i] = black;  //A-G
  }
}

/**
 7 Segment display
 very simple implementation of 7segment display
  //
  //      A
  //     ---
  //  F |   | B
  //     -G-
  //  E |   | C
  //     ---
  //      D
  @param uint8_t no number to be displayed on 7seg display
 */
void d7seg_show(uint8_t no)
{
  switch(no)
  {
    case 0:
      colors[diff] = color;    //A
      colors[diff+1] = color;  //B
      colors[diff+2] = color;  //C
      colors[diff+3] = color;  //D
      colors[diff+4] = color;  //E
      colors[diff+5] = color;  //F
      break;
    case 1:
      colors[diff+1] = color;  //B
      colors[diff+2] = color;  //C
      break;
    case 2:
      colors[diff+0] = color;  //A
      colors[diff+1] = color;  //B
      colors[diff+3] = color;  //D
      colors[diff+4] = color;  //E
      colors[diff+6] = color;  //G
      break;
    case 3:
      colors[diff+0] = color;  //A
      colors[diff+1] = color;  //B
      colors[diff+2] = color;  //C
      colors[diff+3] = color;  //D
      colors[diff+6] = color;  //G
      break;
    case 4:
      colors[diff+1] = color;  //B
      colors[diff+2] = color;  //C
      colors[diff+5] = color;  //F
      colors[diff+6] = color;  //G
      break;
    case 5:
      colors[diff]   = color;  //A
      colors[diff+2] = color;  //C
      colors[diff+3] = color;  //D
      colors[diff+5] = color;  //F
      colors[diff+6] = color;  //G
      break;
    case 6:
      colors[diff]   = color;  //A
      colors[diff+2] = color;  //C
      colors[diff+3] = color;  //D
      colors[diff+4] = color;  //E
      colors[diff+5] = color;  //F
      colors[diff+6] = color;  //G
      break;
    case 7:
      colors[diff]   = color;  //A
      colors[diff+1] = color;  //B
      colors[diff+2] = color;  //C
      break;
    case 8:
      colors[diff] = color;    //A
      colors[diff+1] = color;  //B
      colors[diff+2] = color;  //C
      colors[diff+3] = color;  //D
      colors[diff+4] = color;  //E
      colors[diff+5] = color;  //F
      colors[diff+6] = color;  //G
      break;
    case 9:
      colors[diff]   = color;  //A
      colors[diff+1] = color;  //B
      colors[diff+2] = color;  //C
      colors[diff+3] = color;  //D
      colors[diff+5] = color;  //F
      colors[diff+6] = color;  //G
      break;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // If any digit is received, we will go into integer parsing mode
  // until all three calls to parseInt return an interger or time out.
  if (Serial.available())
  {
    char c = Serial.peek();
    if (!(c >= '0' && c <= '9'))
    {
      Serial.read(); // Discard non-digit character
    }
    else
    {
      // Read the color from the computer.
      //rgb_color color;
      color.red = Serial.parseInt();
      color.green = Serial.parseInt();
      color.blue = Serial.parseInt();
      //number to be displayed:
      // 0-9 will diplay 0-9
      // but 10-19 will display 0-9 with dot
      ledNo = Serial.parseInt();
      //segment chose [1-2]
      segNo = Serial.parseInt();

      //chosing dipslay [0-1]
      d7seg_Seg_Chose(segNo-1);
      //clear chosen display
      d7seg_clear(); //Clear screen

      //If number is bigger than 9
      if(ledNo > 9)//Display number and dot
      {
        d7seg_show(ledNo-10);
        d7seg_dot(true);
      }
      else //other wise display just number
      {
        d7seg_show(ledNo);
        d7seg_dot(false);
      }
      
      // Write to the LED strip.
      ledStrip.write(colors, LED_COUNT);

      Serial.print("Showing color: ");
      Serial.print(color.red);
      Serial.print(",");
      Serial.print(color.green);
      Serial.print(",");
      Serial.print(color.blue);
      Serial.print(" 7seg: ");
      if(ledNo > 9)
      {
        Serial.print(ledNo-10);
        Serial.print(".");
      }else{
        Serial.print(ledNo);
      }
      Serial.print(" segment: ");
      Serial.println(segNo);
    }
  }
}
