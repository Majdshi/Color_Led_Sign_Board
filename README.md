THIS FILE contain a Project that run on Silicon Lab C8051F380 MCU
USING C LANGUAGE.
the project contain code that make the C8051F380 connect to RTC(ds1307) using I2C protocol (PINS: P1.0-->SCL & P1.1-->SDA).
and a code that make the C8051F380 connect to Temperature and Humidity Sensor(AHT10) USING I2C protocol (PINS: P1.0-->SCL & P1.1-->SDA).
and a code that make the C8051F380 connect to Serial MP3 Player v1.2 using UART protocol(UART1 PINS: P0.0-->TX1 & P0.1-->RX1).
and a code that make the C8051F380 connect to ESP32 MCU using UART protocol(UART0 PINS: P0.4-->TX0 & P0.5-->RX0) and the date are sent from the C8051F380 to the ESP32.
The code display the data on a TFT LCD MODULE 3.95" THAT are connected to the C8051F380 using SPI protocol.
