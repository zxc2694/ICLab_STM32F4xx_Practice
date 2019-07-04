# ICLab_STM32F4xx_Practice

Learning how to develop on the STM32F4-Discovery board

1. Install compiler and tool:
```
 - sudo apt-get install gcc-arm-none-eabi

 - sudo apt-get install automake* libtool libusb-1.0-0-dev
```
2. Install st-link:
```
 - git clone https://github.com/texane/stlink.git
 
 - cd stlink
 
 - git checkout -b c722056 c722056
 
 - ./autogen.sh
 
 - ./configure --prefix=/usr
 
 - make
 
 - sudo make install
 
 - sudo cp 49-stlinkv2.rules /etc/udev/rules.d/
 
```
3. Install openocd:
```
 - https://github.com/ntfreak/openocd
 
 - cd openocd
 
 - ./bootstrap
 
 - ./configure --prefix=/usr/local  --enable-jlink --enable-amtjtagaccel --enable-buspirate  --enable-stlink   --disable-libftdi
 
 - echo -e "all:\ninstall:" > doc/Makefile
 
 - make
 
 - sudo make install
```
