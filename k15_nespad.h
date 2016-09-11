/*
  K15 NES Pad v 1.0
  Single header public domain library

# Author(s):
  Felix Klinge (fklinge dot deck13 dot com)

# Version history
  1.0 | 9/11/2016   - Intial release

# What problem is this library trying to solve? (ELI5)
  This library can be used to get informations about what buttons
  are pressed on a connected NES controller pad. You can use this to
  control your Arduino projects using one or many NES controller.

# How do I add this library to my project?
  This library is a single header library which means that you just have to
  #include it into your project. However, you have to add the implementation
  of the functions used into *one* of your C/CPP/INO files. To do this you have
  to add #define K15_NESPAD_IMPLEMENTATION *before* the #include.
  e.g:
    #define K15_NESPAD_IMPLEMENTATION
    #include "K15_NESPad.h"
  in ONE of you C/CPP/INO files.

# How does this library work?
  This library uses 3 pins to interact with any given NES controller.
  2 pins are used for output (latch and pulse) and 1 pin is used as
  input (data). I implemented this library following the great
  article from Tresi Avrizo about how the controller works.
  The article can be found using this link: http://www.mit.edu/~tarvizo/nes-controller.html

  Basically the library sends several signals via the pulse pin to prompt the controller
  to send signals via the data pin which indicates whether a given button has been pressed
  or not. For any series of pulse signals, a latch signal has to be sent first.

  Be also sure to connect +5v and GND to your NES controller.

  Pinout (from Tresi Avrizo's article):
            +----> Power  (white)
            |
  5 +---------+  7    
    | x  x  o   \     
    | o  o  o  o |    
  4 +------------+ 1  
      |  |  |  |
      |  |  |  +-> Ground (brown)
      |  |  +----> Pulse  (red)
      |  +-------> Latch  (orange)
      +----------> Data   (yellow)


# What pins can I use on my Arduino?
  The pins for the latch, data and pulse signal have to be digital pins.
  

# Example Usage
  K15_NESPad nesPad;

  void setup()
  {
    //Setup the library.
    //pulePin, latchPin and dataPin are all digital pins.
    int pulsePin = 8;
    int latchPin = 7;
    int dataPin = 12;
    K15_NESInitPad(&nesPad, pulsePin, dataPin, latchPin);

    //Intial Serial Monitor so we can use Serial.println
    Serial.begin(9600);
  }

  void loop()
  {
    //Get the button state from the controller
    K15_NESPollButtons(&pad);

    //Check which buttons have been pressed
    if (K15_NESButtonPressed(&pad, K15_NESPadButton_A))
      Serial.println("A pressed!");
    if (K15_NESButtonPressed(&pad, K15_NESPadButton_B))
      Serial.println("B pressed!");
    if (K15_NESButtonPressed(&pad, K15_NESPadButton_Start))
      Serial.println("Start pressed!");
    if (K15_NESButtonPressed(&pad, K15_NESPadButton_Select))
      Serial.println("Select pressed!");
    if (K15_NESButtonPressed(&pad, K15_NESPadButton_Left))
      Serial.println("Left pressed!");
    if (K15_NESButtonPressed(&pad, K15_NESPadButton_Right))
      Serial.println("Right pressed!");
    if (K15_NESButtonPressed(&pad, K15_NESPadButton_Up))
      Serial.println("Up pressed!");
    if (K15_NESButtonPressed(&pad, K15_NESPadButton_Down))
      Serial.println("Down pressed!");
  }

  # License:
    This software is in the public domain. Where that dedication is not
    recognized, you are granted a perpetual, irrevocable license to copy
    and modify this file however you want.
  */

typedef struct
{
  unsigned char buttonMask;
  unsigned char pulsePin;
  unsigned char dataPin;
  unsigned char latchPin;
} K15_NESPad;

typedef enum
{
  K15_NESPadButton_A = 0x01,
  K15_NESPadButton_B = 0x02,
  K15_NESPadButton_Select = 0x04,
  K15_NESPadButton_Start = 0x08,
  K15_NESPadButton_Up = 0x10,
  K15_NESPadButton_Down = 0x20,
  K15_NESPadButton_Left = 0x40,
  K15_NESPadButton_Right = 0x80
} K15_NESPadButton;

void K15_NESInitPad(K15_NESPad* p_Pad, int p_PulsePin, int p_DataPin, int p_LatchPin);
void K15_NESPollButtons(K15_NESPad* p_Pad);
int K15_NESButtonPressed(K15_NESPad* p_Pad);

#ifdef K15_NESPAD_IMPLEMENTATION

void K15_NESInitPad(K15_NESPad* p_Pad, int p_PulsePin, int p_DataPin, int p_LatchPin)
{
  if (!p_Pad)
    return;

  p_Pad->buttonMask = 0;
  p_Pad->pulsePin = p_PulsePin;
  p_Pad->dataPin = p_DataPin;
  p_Pad->latchPin = p_LatchPin;

  pinMode(p_DataPin, INPUT);
  pinMode(p_PulsePin, OUTPUT);
  pinMode(p_LatchPin, OUTPUT);
}

void K15_NESPollButtons(K15_NESPad* p_Pad)
{
  if (!p_Pad)
    return;

  int buttonMask = 0;
  digitalWrite(p_Pad->latchPin, HIGH);
  digitalWrite(p_Pad->latchPin, LOW);
  delayMicroseconds(6);

  for (int buttonIndex = 0; buttonIndex < 8; ++buttonIndex)
  {
      int dataPulse = digitalRead(p_Pad->dataPin);
      digitalWrite(p_Pad->pulsePin, HIGH);
      digitalWrite(p_Pad->pulsePin, LOW);

      if (dataPulse == LOW)
        buttonMask |= (1 << buttonIndex);
  }

  p_Pad->buttonMask = buttonMask;
}

int K15_NESButtonPressed(K15_NESPad* p_Pad, K15_NESPadButton p_Button)
{
  if (!p_Pad)
    return 0;

  return (p_Pad->buttonMask & p_Button) > 0;
}

#endif //K15_NESPAD_IMPLEMENTATION
