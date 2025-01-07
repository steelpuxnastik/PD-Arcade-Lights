[![Stand With Ukraine](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/banner-direct.svg)](https://vshymanskyy.github.io/StandWithUkraine)

# PD ArcadeLights
Plugin for [PD Loader](https://github.com/PDModdingCommunity/PD-Loader) that sends button LEDs and LEDs on the sides of display (in original arcade cabinet) status to the receiver over UART, that will output this light for you.
As receiver you can use:
1. [Arduino example board](https://github.com/steelpuxnastik/PD-Arcade-Lights/tree/master/PCB) as reference point or as "ready device as is".
2. [PD Arcade picohub](https://github.com/toha-cat/PD_Arcade_picohub) as arcade [controller board](https://github.com/toha-cat/PD_Arcade_picohub/tree/main/PCB) with integrated ArcadeLights possibility.
**I draw your attention to the need to connect the microcontroller board in accordance with the diagram, because the firmware is designed to work with these components. If you understand what and how you want to change this - change it.**

![Settings window](/images/ArcadeLightsSettingsWindow.png)

## Features
 - possibility to choose COM-port of your receiver, baudrate and delay of data receiving;
 - possibility to change brightness of partition lights;
 - possibility to ignore partition lights data (partition lights will be turned off, only buttons will work);
 - after closing the game, LEDs automatically turns off;

## Installing
Put `ArcadeLighs.dva` plugin into the `plugins` folder in the PDAFT game folder. Configuration file will be created automatically after you save the settings in **PD Loader**.

## License
  Copyright (c) Project DIVA Modding 2nd Community  
  Copyright (c) [COM14](https://github.com/COM14), [steelpuxnastik](https://github.com/steelpuxnastik), [toha-cat](https://github.com/toha-cat)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

