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

------------------
issue1:
```
/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv7e-m/softfp/libc.a(lib_a-exit.o): In function `exit':
/build/newlib-5zwpxE/newlib-2.2.0+git20150830.5a3d536/build/arm-none-eabi/armv7e-m/softfp/newlib/libc/stdlib/../../../../../../../newlib/libc/stdlib/exit.c:70: undefined reference to `_exit'
collect2: error: ld returned 1 exit status
Makefile:73: recipe for target 'stm32_executable.elf' failed
make: *** [stm32_executable.elf] Error 1
```

solution:

https://stackoverflow.com/questions/13235748/linker-error-on-a-c-project-using-eclipse

create libs.ld file
```
GROUP(
   libgcc.a
   libg.a
   libc.a
   libm.a
   libnosys.a
 )
 ```
 
Makefile modify:

#stm32-flash

CFLAGS+=-Wl,-T,libs.ld,-T,stm32_flash.ld
