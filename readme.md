Parts List
---

From muurkha:

- one or two of the Ambiq Apollo3 devboards (probably not the
  Arduino-sized one);
- two of the 400×240 memory-in-pixel displays; and
- one or more of the Cypress S34MS01G200TFI900 gibibit Flash chips.

Purchased list:

- $21.50 Ambiq Apollo3 SoC breakout dev board. Sparkfun: [Sparkfun DEV-15444][1] (x1)
    - [Apollo3 Blue Datasheet][6] [(ambiq)][7]
    - [Cortex M4 Datasheet][18] [Tech Manual][19]
    - [Schematic][8]
    - [Integration Guide][9]
    - [Arduino Support][10]
- $9.95 Ambiq Apollo3 SoC module. Sparkfun: [Sparkfun WRL-15484][2] (x2)
    - [Apollo3 Blue Datasheet][6] [(ambiq)][7]
    - [Cortex M4 Datasheet][18] [Tech Manual][19]
    - [Integration Guide][9]
    - [Schematic][11]
    - [Pinout][12]
- $44.95 Adafruit SHARP Memory Display Breakout - 2.7" 400x240. Adafruit: [Adafruit 4694][3] (x2)
    - [Adafruit Guide][13]
    - [Datasheet][14]
    - [Arduino Library][15]
- $6.46 4 GBit Parallel NAND (45ns). Digi-key: [S34MS04G204TFI010][4] (x2)
    - Note: 1Gbit is more like $1 but out of stock atm.
    - [Datasheet][16]
- $21.90 Low-power radio (optional). WIO-E5 MINI DEV BRD STM32WLE5JC. Digi-key, seeed studio: [Seeed 113990939][5] (x1)
    - [Datasheet][17]

[1]: https://www.sparkfun.com/products/15444
[2]: https://www.sparkfun.com/products/15484
[3]: https://www.adafruit.com/product/4694
[4]: https://www.digikey.com/en/products/detail/cypress-semiconductor-corp/S34MS04G204TFI010/4457680
[5]: https://www.digikey.com/en/products/detail/seeed-technology-co-ltd/113990939/13926228

[6]: https://cdn.sparkfun.com/assets/1/5/c/6/7/Apollo3-Blue-MCU-Datasheet_v0_15_0.pdf
[6]: datasheets/Apollo3-Blue-SoC_Datasheet.pdf
[7]: https://ambiq.com/wp-content/uploads/2020/10/Apollo3-Blue-SoC-Datasheet.pdf
[8]: https://cdn.sparkfun.com/assets/4/5/a/3/e/RedBoardArtemisSchematic.pdf
[8]: datasheets/Artemis-RedBoard_Schematic.pdf
[9]: https://cdn.sparkfun.com/assets/8/7/5/3/f/Artemis_Integration_Guide.pdf
[10]: https://github.com/sparkfun/Arduino_Apollo3
[11]: https://cdn.sparkfun.com/assets/5/c/b/3/c/SparkFun_Artemis.pdf
[11]: datasheets/Artemis_Schematic.pdf
[12]: https://cdn.sparkfun.com/assets/9/a/7/1/c/ArtemisModule_Pinout.pdf
[12]: datasheets/Artemis_Pinout.pdf
[18]: https://documentation-service.arm.com/static/62053f0a0ca305732a3a5c17?token=
[18]: datasheets/Arm-Cortex-M4_Datasheet.pdf
[19]: https://documentation-service.arm.com/static/5f19da2a20b7cf4bc524d99a?token=
[19]: datasheets/Arm-Cortex-M4_Manual.pdf

[13]: https://learn.adafruit.com/adafruit-sharp-memory-display-breakout
[14]: https://cdn-learn.adafruit.com/assets/assets/000/094/215/original/LS027B7DH01_Rev_Jun_2010.pdf?1597872422
[14]: datasheets/Sharp_LCD_Datasheet.pdf
[15]: https://github.com/adafruit/Adafruit_SHARP_Memory_Display

[16]: https://media.digikey.com/pdf/Data%20Sheets/Cypress%20PDFs/S34MS01G2_2G2_4G2_RevE_6-9-17.pdf
[16]: datasheets/Flash_Datasheet.pdf

[17]: https://files.seeedstudio.com/products/317990687/res/LoRa-E5%20module%20datasheet_V1.0.pdf
[17]: datasheets/Radio_Datasheet.pdf
