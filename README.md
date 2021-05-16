# Birdhouse Peeper

An IoT sensor, based on Espressif, that monitors a birdhouse's inside.

Current implementation is based on these:
- the esp-who's camera-web-server example (without the face detection).
- http-request (repetitively sending an captured image to a hard-coded IP address and port)
  Current IP/port is hard-coded as 192.168.40.41/3334 (NUC)
  Use this script to receive the http request: `python3 http_server.py 3334`
  Note: HTTPS does not work properly, http2.0 did not work in python

It depends on the external repo esp32-camera.

## AI-thinker esp32-cam

https://randomnerdtutorials.com/esp32-cam-ai-thinker-pinout/

```bash
cp sdkconfig.ai-thinker.defaults sdkconfig
rm -rf build
```

## esp-eye

```bash
cp sdkconfig.esp32-cam.defaults sdkconfig
rm -rf build
```

## Building

Use one of the sdkconfig defaults

```bash
idf.py menuconfig # to configure AP SSID and passphrase (also for station mode)
idf.py build
idf.py flash
idf.py monitor
```

## State

## Pin usage

### AI-thinker esp32-cam based on AI-thinker esp32-s module

https://github.com/raphaelbs/esp32-cam-ai-thinker/blob/master/docs/esp32cam-pin-notes.md


|NO|Name     | GPIO |Camera |EXTRA| SDcard  |Pin |Function               |
+--+---------+------+-------+-----+---------+----+-----------------------|
| 1|GND      |Gnd   |       |     |         |    |
| 2|3V3      |Power |       |     |         |   1|
| 3|EN       |CS    |       |RST B|         |    |
| 4|SENSOR_VP|GPIO36|D4     |     |         |    |SENSOR_VP,ADC_H,ADC1_CH0,RTC_GPIO0
| 5|SENSOR_VN|GPIO39|D5     |     |         |    |SENSOR_VN,ADC1_CH3,ADC_H,RTC_GPIO3
| 6|IO34     |GPIO34|D6     |     |         |    |ADC1_CH6,RTC_GPIO4
| 7|IO35     |GPIO35|D7     |     |         |    |ADC1_CH7,RTC_GPIO5
| 8|IO32     |GPIO32|POWER  |     |         |    |XTAL_32K_P (32.768 kHz crystal oscillator input),ADC1_CH4,TOUCH9,RTC_GPIO9
| 9|IO33     |GPIO33|       |LED  |         |    |XTAL_32K_N (32.768 kHz crystal oscillator output),ADC1_CH5,TOUCH8,RTC_GPIO8
|10|IO25     |GPIO25|VSYNC  |     |         |    |DAC_1,ADC2_CH8,RTC_GPIO6,EMAC_RXD0
|11|IO26     |GPIO26|TWI SDA|     |         |    |DAC_2,ADC2_CH9,RTC_GPIO7,EMAC_RXD1
|12|IO27     |GPIO27|TWI SCL|     |         |    |ADC2_CH7,TOUCH7,RTC_GPIO17,EMAC_RX_DV
|13|IO14     |GPIO14|       |     |HS2_CLK  |6   |ADC2_CH6,TOUCH6,RTC_GPIO16,MTMS,HSPICLK,HS2_CLK,SD_CLK,EMAC_TXD2
|14|IO12     |GPIO12|       |     |HS2_DATA2|3   |ADC2_CH5,TOUCH5,RTC_GPIO15,MTDI,HSPIQ,HS2_DATA2,SD_DATA2,EMAC_TXD3
|15|GND      |Gnd   |       |     |         |    |
|16|IO13     |GPIO13|       |     |HS2_DATA3|4   |ADC2_CH4,TOUCH4,RTC_GPIO14,MTCK,HSPID,HS2_DATA3,SD_DATA3,EMAC_RX_ER
|17|SHD/SD2  |GPIO9 |       |     |         |    |SD_DATA2,SPIHD,HS1_DATA2,U1RXD
|18|SHD/SD3  |GPIO10|       |     |         |    |SD_DATA3,SPIWP,HS1_DATA3,U1TXD
|19|SCS/CMD  |GPIO11|       |     |         |    |SD_CMD,SPICS0,HS1_CMD,U1RTS
|20|SCK/CLK  |GPIO6 |       |     |         |    |SD_CLK,SPICLK,HS1_CLK,U1CTS
|21|SDO/SD0  |GPIO7 |       |     |         |    |SD_DATA0,SPIQ,HS1_DATA0,U2RTS
|22|SDI/SD1  |GPIO8 |       |     |         |    |SD_DATA1,SPID,HS1_DATA1,U2CTS
|23|IO15     |GPIO15|       |     |HS2_CMD  |5   |ADC2_CH3,TOUCH3,MTDO,HSPICS0,RTC_GPIO13,HS2_CMD,SD_CMD,EMAC_RXD3
|24|IO2      |GPIO2 |       |     |HS2_DATA0|7   |ADC2_CH2,TOUCH2,RTC_GPIO12,HSPIWP,HS2_DATA0,SD_DATA0
|25|IO0      |GPIO0 |MCLK   |     |         |   3|ADC2_CH1,TOUCH1,RTC_GPIO11,CLK_OUT1,EMAC_TX_CLK
|26|IO4      |GPIO4 |       |FLASH|HS2_DATA1|8   |ADC2_CH0,TOUCH0,RTC_GPIO10,HSPIHD,HS2_DATA1,SD_DATA1,EMAC_TX_ER
|27|IO16     |GPIO16|       |     |         |   2|HS1_DATA4,U2RXD,EMAC_CLK_OUT
|28|IO17     |GPIO17|       |     |         |    |HS1_DATA5,U2TXD,EMAC_CLK_OUT_180
|29|IO5      |GPIO5 |D0     |     |         |    |VSPICS0,HS1_DATA6,EMAC_RX_CLK
|30|IO18     |GPIO18|D1     |     |         |    |VSPICLK,HS1_DATA7
|31|IO19     |GPIO19|D2     |     |         |    |VSPIQ,U0CTS,EMAC_TXD0
|32|NC       |-     |       |     |         |    |
|33|IO21     |GPIO21|D3     |     |         |    |VSPIHD,EMAC_TX_EN
|34|RXD0     |GPIO3 |       |     |         |   6|U0RXD,CLK_OUT2
|35|TXDO     |GPIO1 |       |     |         |   7|U0TXD,CLK_OUT3,EMAC_RXD2
|36|IO22     |GPIO22|PCLK   |     |         |    |VSPIWP,U0RTS,EMAC_TXD1
|37|IO23     |GPIO23|HSYNC  |     |         |    |VSPID,HS1_STROBE
|38|GND      |Gnd   |       |     |         |    |

### Forgot which device this is about

| Module | Func     | Camera    |  SD-Card  |   PSRAM   | Pin   | Comment
+--------+----------+-----------+-----------+-----------|-------+
|    1   |  GND     |           |           |           |       |
|    2   |  3V3     |           |           |           |       |
|    3   |   EN     |           |           |           |       |
|    4   |   VP     | CSI_D4    |           |           |       |
|    5   |   VN     | CSI_D5    |           |           |       |
|    6   | IO34     | CSI_D6    |           |           |       |
|    7   | IO35     | CSI_D7    |           |           |       |
|    8   | IO32     |           |           |           |       |
|    9   | IO33     |           |           |           |       |
|   10   | IO25     | CSI_VSYNC |           |           |       |
|   11   | IO26     | TWI_SDA   |           |           |       |
|   12   | IO27     | TWI_SCK   |           |           |       |
|   13   | IO14     |           | HS2_CLK   |           |  6    | MTMS HSPICLK HS2_CLK SD_CLK EMAC_TXD2
|   14   | IO12     |           | HS2_DATA2 |           |  3    | MTDI HSPIQ HS2_DATA2 SD_DATA2 EMAC_TXD3
|   15   |  GND     |           |           |           |       |
|   16   | IO13     |           | HS2_DATA3 |           |  4    | MTCK HSPID HS2_DATA3 SD_DATA3 EMAC_RX_ER
|   17   |  SD2     |           |           | SD2       |       |
|   18   |  SD3     |           |           | SD3       |       |
|   19   |  CMD     |           |           |           |       |
|   20   |  CLK     |           |           |           |       |
|   21   |  SD0     |           |           | SD0       |       |
|   22   |  SD1     |           |           | SD1       |       |
|   23   | IO15     |           | HS2_CMD   |           |  5    | MTDO HSPICS0 HS2_CMD SD_CMD EMAC_RXD3
|   24   |  IO2     |           | HS2_DATA0 |           |  7    | HSPIWP HS2_DATA0 SD_DATA0
|   25   |  IO0     | CSI_MCLK  |           |           |    14 | CLK_OUT1 (I2S), EMAC_TX_CLK
|   26   |  IO4     |           | HS2_DATA1 |           |  8    | HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
|   27   | IO16     |           |           | CS        |    15 | U2RXD, EMAC_CLK_OUT
|   28   | IO17     |           |           | PSRAM_CLK |       |
|   29   |  IO5     | CSI_D0    |           |           |       |
|   30   | IO18     | CSI_D1    |           |           |       |
|   31   | IO19     | CSI_D2    |           |           |       |
|   32   |          |           |           |           |       |
|   33   | IO21     | CSI_D3    |           |           |       |
|   34   | IO3 U0RX |           |           |           |    11 | CLK_OUT2 (I2S)
|   35   | IO1 U0TX |           |           |           |    10 | CLK_OUT3 (I2S), EMAC_RXD2
|   36   | IO22     | CSI_PCLK  |           |           |       |
|   37   | IO23     | CSI_HSYNC |           |           |       |
|   38   |  GND     |           |           |           |       |
