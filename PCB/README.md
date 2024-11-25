# ArcadeLights PCB example
=================================
![PCB](/PCB/images/pcb.png)
## Parts list
- PCB: 1x `ArcadeLight_uart_board` **1.6mm thickness** (KiCad 7.99+ (7.99.0-697-g4d05bd62da), release build required for exporting Gerber files)
  - U1: `Arduino Nano` board
  - U2, U3: 2x `ULN2003` transistor assembly
- Parts for PWM brightness control (short jumpers if no PWM used, **brightness control will not work**):
  - Q1: `IRFR9024N` P-channel MOSFET
  - Q2: `AO3404A` N-channel MOSFET
  - R1: `1k` 1206 SMD resistor
  - R2: `10k` 1206 SMD resistor
- connectors:
  - 2x DIP-16 panel for `ULN2003`
  - 4x `JST XH-02` male connector
  - 3x `JST XH-04` male connector
  - female 2.54 single row headset for `Arduino Nano`