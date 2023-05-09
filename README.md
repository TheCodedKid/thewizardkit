# The Wizard Kit

[![Build passing](https://img.shields.io/badge/build-passing-brightgreen.svg)]()

The idea of this project is to create a shell/driver combination to create a secure way to use Linux. Utilizing a RP2040 basic board as the "key" or "wizard staff", the linux module is able to see and signal probe and disconnect events. From this, we created a shell that listens to those events, and creates an accessible shell only if the device is connected. 

```"								  + 		  *    =
								=  *		- 	+ 	*   *
							   -   (\\.   \\   *  ,/)     -
  _____ _____ _____ _____     _     \\(   |\\     )/\
 |     |  _  |   | |  _  |___| |_   //\\  | \\   /\\\\
 | | | |     | | | |     |_ -|   | (/ /\\_#oo#_/\\ \\)
 |_|_|_|__|__|_|___|__|__|___|_|_|  \\/\\  ####  /\\/
		Wizardry, at a cost		           `##'
```

## Authors

- [@RaidtoRaider](https://github.com/RaidToRadar)
- [@TheCodedKid](https://github.com/TheCodedKid)
## Acknowledgements

- [Writing a Simple USB Driver](https://www.linuxjournal.com/article/7353)
- [Writing USB Device Drivers](https://docs.kernel.org/driver-api/usb/writing_usb_driver.html)
- [USB Device Driver Example – Linux Device Driver Tutorial Part 34](https://embetronicx.com/tutorials/linux/device-drivers/usb-device-driver-example/)
- [USB-driver](https://github.com/AcollaMolla/USB-driver)
