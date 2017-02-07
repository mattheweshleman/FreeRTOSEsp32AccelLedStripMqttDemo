# README #

This repo contains the source code for a system of MQTT networked ESP32 based devices used to demonstrate various RTOS concepts for a local Nashville TN microcontroller meetup. The original event details are here:
https://www.meetup.com/NashMicro/events/236602045/

Additionally, it was fun to play around and learn more about the ESP32 and MQTT.

Features:

* A handful of fun LED Strip patterns
* Builds including an Accelerometer will issue an MQTT event when tapped/bumped and also light up the LED Strip.
* All builds subscribe to a Pattern MQTT event, enabling the ability to remotely change the LED Strip lighting pattern.
* Builds without an accelerometer subscribe to an Accelerometer topic, and will light up their LED Strip when that topic is hit.
* Some code modules written specifically to demonstrate various FreeRTOS concepts. Listed later in this readme document.

## Target Hardware ##

### Components ###

* [Sparkfun ESP32 Thing](https://www.sparkfun.com/products/13907)
* [Adafruit NeoPixel Digital RGB LED Strip - Black 60 LED](https://www.adafruit.com/products/1461)
* Code as published assumes 60 LEDs per device, but is easy to change.
* [ADXL345 Accelerometer](https://www.adafruit.com/products/1231)
* Build supports with or without accelerometer, using "make menuconfig" to change.

### Wiring / GPIO usage ###
* GPIO 5: LED and crude CPU usage monitor
* GPIO 21: LED Strip data connection
* GPIO 16: I2C-SDA for ADXL345
* GPIO 17: I2C-SCL for ADXL345

It is assumed (and recommended) that the LED Strip itself is externally powered, due to heavy peak current demands with some LED patterns.

## Setup ##

### Minimum Requirements ###

* [ESP-IDF](https://github.com/espressif/esp-idf) installed correctly on the development system. This code was tested against the latest IDF as of end of December 2016. *NOTE: The ESP-IDF is a rapidly moving target. Your mileage may vary.*
* FWIW, development for this project was performed using Mac OSX, and using Eclipse for editing
* I used the ESP32 Thing's built in serial bootloader to download new application code builds using the IDF's "make flash" feature. For faster downloads, I highly suggest using "make menuconfig" to increase the bootloader's serial bit rate AND use compression.

### Configuration ###

#### Accelerometer ####
* Use "make menuconfig" to enable or disable the accelerometer option. See "Component Config -> EshThing" options.

#### Wifi and MQTT ####

* The wifi setup may be updated via "make menuconfig". See "Component Config"->"EshThing" to update SSID and password.
* The target MQTT server may be updated via "make menuconfig". See "Component Config"->"EshThing" to update the target MQTT server.
* The MQTT clientID may be updated via "make menuconfig". See "Component Config"->"EshThing" to update the MQTT ClientId.

#### MQTT Topics ####

* "/EshThings/Events/Pattern"
  *  All builds subscribe to this topic. A simple string/number is expected:
    * '0' none. Pattern is off.
    * '1' Hann Window pulse. Single pulse and then off.
    * '2' Hann Window repeating pulse
    * '3' Random noise using ESP32 random generator source
    * '4' SOS Morse Code pattern pulse. Single pulse and then off.
    * '5' Same as '1', but scaled by 0.5
    * '6' Same as '2', but scaled by 0.5
    * '7' Knight Rider
* "/EshThings/Events/Accelerometer"
  * Builds *WITHOUT* a physical accelerometer will subscribe to this topic. Any event will trigger LED behavior.
  * Builds *WITH* a physical accelerometer will publish to this topic when a threshold of accelerometer data is crossed. The published data includes the magnitude information.
  

## Teaching Demo ##

The purpose of this repository is to present various FreeRTOS concepts at a microcontroller meetup in Nashville TN. The concepts and their locations:

* Semaphores: led_strip (third party)
* Mutex: led_strip (third party as modified by Matthew)
* Queues: ColorMappedDataVisualizer (original to this code)
* Timers: PatternGenerator (original to this code)
* Polling Thread: AccelReader (original to this code)

## Third Party ##

Third party code and modules required or included directly in this project:

* [ESP-IDF](https://github.com/espressif/esp-idf) 
* [ESP32_LED_STRIP aka led_strip](https://github.com/Lucas-Bruder/ESP32_LED_STRIP) 
* [esp32-i2c-adxl345](https://github.com/imxieyi/esp32-i2c-adxl345) 
* [esp32-mqtt](https://github.com/tuanpmt/esp32-mqtt)

Other resources:

* [Code - heatmaps and color gradients](http://www.andrewnoske.com/wiki/Code_-_heatmaps_and_color_gradients)


## Who do I talk to? ##

* Feel free to contact [Matthew Eshleman](https://covemountainsoftware.com/consulting/) if any interest or issues with this code.
* Reach out to the [ESP32 community](http://esp32.com/) for ESP32 or ESP32-IDF specific issues.

