I needed a page turner pedal for my tablet when playing music with friends. Buying one was too expensive so I made one with an ESP32. There are lots of examples around but the code is fairly trivial. I wanted a battery-powered one for use at gigs and jams and so used a board that allowed charging and monitoring of the battery.

I used a green LED for "Charge" and a red LED for "Not connected". As these have lower forward voltages than the blue LEDsk already on the board, I left the original LEDs in place and just connected across the two existing blue LEDs (one is in the charge circuit, the other is the LED_BUILTIN found on most boards.

You can find STL files for the case I built it into at: https://www.thingiverse.com/thing:4880077

I used the following components to complete the project:

Pedal: https://www.aliexpress.com/item/32801886725.html

ESP32: https://www.aliexpress.com/item/4000830795113.html

Battery: https://www.aliexpress.com/item/1005001602175264.html

Chassis mount USB extension: https://www.ebay.co.uk/itm/232481417132
