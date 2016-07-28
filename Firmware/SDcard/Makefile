#======================================================================#
#Output files
EXECUTABLE=stm32_executable.elf
BIN_IMAGE=stm32_bin_image.bin

#======================================================================#
#Cross Compiler
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

#======================================================================#
#Flags
CFLAGS=-g -mlittle-endian -mthumb -lgcc -lc -lm
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS+=-D USE_STDPERIPH_DRIVER
CFLAGS+=-I./


#stm32-flash
CFLAGS+=-Wl,-T,stm32_flash.ld

#======================================================================#
#Libraries

#Stm32 libraries
ST_LIB=../Libraries/STM32F4xx_StdPeriph_Driver
FATFS_LIB=../Libraries/fatfs
FATFS_D_LIB=../Libraries/fatfs/drivers
FATFS_O_LIB=../Libraries/fatfs/option
CFLAGS+=-I../Libraries/CMSIS/ST/STM32F4xx/Include

#CMSIS libraries
CFLAGS+=-I../Libraries/CMSIS/Include

#StdPeriph includes
CFLAGS+=-I$(ST_LIB)/inc
CFLAGS+=-I$(FATFS_LIB)
CFLAGS+=-I$(FATFS_LIB)/drivers
#======================================================================#
#Source code
SRC=../Libraries/CMSIS/system_stm32f4xx.c

#StdPeriph
SRC+=$(ST_LIB)/src/misc.c \
	$(ST_LIB)/src/stm32f4xx_rcc.c \
	$(ST_LIB)/src/stm32f4xx_dma.c \
	$(ST_LIB)/src/stm32f4xx_flash.c \
	$(ST_LIB)/src/stm32f4xx_gpio.c \
	$(ST_LIB)/src/stm32f4xx_usart.c \
	$(ST_LIB)/src/stm32f4xx_tim.c\
	$(ST_LIB)/src/stm32f4xx_spi.c\
	$(ST_LIB)/src/tm_stm32f4_gpio.c\
	$(ST_LIB)/src/stm32f4xx_sdio.c\
	$(ST_LIB)/src/tm_stm32f4_spi.c\
	$(ST_LIB)/src/tm_stm32f4_delay.c\
	$(ST_LIB)/src/tm_stm32f4_disco.c\
	$(ST_LIB)/src/tm_stm32f4_fatfs.c\
	$(ST_LIB)/src/string.c\
	$(ST_LIB)/src/stdlib.c\
	$(FATFS_O_LIB)/syscall.c\
	$(FATFS_O_LIB)/unicode.c\
	$(FATFS_D_LIB)/fatfs_sd.c\
	$(FATFS_D_LIB)/fatfs_sd_sdio.c\
	$(FATFS_LIB)/diskio.c\
	$(FATFS_LIB)/ff.c
#Major programs
SRC+=./main.c

#======================================================================#
#STM32 startup file
STARTUP=./startup_stm32f4xx.s

#======================================================================#
#Make rules

#Make all
all:$(BIN_IMAGE)

$(BIN_IMAGE):$(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@

STARTUP_OBJ = startup_stm32f4xx.o

$(STARTUP_OBJ): $(STARTUP) 
	$(CC) $(CFLAGS) $^ -c $(STARTUP)

$(EXECUTABLE):$(SRC) $(STARTUP_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

#Make clean
clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
#Make flash
flash:
	st-flash write $(BIN_IMAGE) 0x8000000

#======================================================================
.PHONY:all clean flash
