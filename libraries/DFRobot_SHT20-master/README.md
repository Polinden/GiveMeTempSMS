# DFRobot_SHT20
* [中文版](./README_CN.md)

This is a SHT20 I2C temperature & humidity sensor with waterproof probe.
It comes with the 4C CMOSens® SHT20 temperature & humidity sensor chip, and the probe has gone through dual waterproof protection.
The SHT20 I2C temperature & humidity sensor adopt Sensirion new technique.
Besides the capacitive type humidity sensor and the band gap temperature sensor, the chip contains an amplifier, A/D converter, OTP memory and a digital processing unit.
To compare with early SHT1x series and SHT7x series, SHT20 shows better reliability and long-term stability.
It can measure surrounding environment temperature and relative air humidity precisely.


![产品实物图](./resources/images/SHT20.png)


## Product Link (https://www.dfrobot.com/product-1636.html)
    SKU: SEN0227


## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary

* The Arduino SHT20 waterproof temperature & humidity sensor adopts dual waterproof protection.
* The inner PCB has perfusion and encapsulation protection, and the probe enclosure is made of PE waterproof materials.
* This is a special waterproof breathable material that allows water molecules to seep in, blocking water droplets from seeping in.
* The sensor won't be damaged even if it is submerged in water for a long time.
* There is a built-in 10k Pull-up resistor and 0.1uf filter capacitor, so It can be used directly with the microcontroller such as Arduino.
* Recommend DFRobot Gravity 4Pin Sensor Adapter, it is quite convenient.


## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, 
then open the examples folder and run the demo in the folder.


## Methods

```C++

  /**
   * @fn initSHT20
   * @brief 初始化函数
   * @return None
   */
  void initSHT20(void);

  /**
   * @fn readHumidity
   * @brief 读取空气湿度测量数据
   * @return 返回float类型的空气湿度测量数据, 单位: %
   */
  float readHumidity(void);

  /**
   * @fn readTemperature
   * @brief 读取温度测量数据
   * @return 返回float类型的温度测量数据, 单位: C
   */
  float readTemperature(void);

  /**
   * @fn checkSHT20
   * @brief 检测SHT20当前状态信息
   * @n 状态信息包括: End of battery, Heater enabled, Disable OTP reload
   * @n 检测结果包括: yes, no
   * @return None
   */
  void checkSHT20(void);

```


## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | :----:
Arduino Uno        |      √       |              |             |
Arduino MEGA2560   |      √       |              |             |
Arduino Leonardo   |      √       |              |             |
FireBeetle-ESP8266 |      √       |              |             |
FireBeetle-ESP32   |      √       |              |             |
FireBeetle-M0      |      √       |              |             |
Micro:bit          |      √       |              |             |


## History

- 2017-9-12 - Version 1.0.0 released.


## Credits

Written by Zhangjiawei(jiawei.zhang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

