
C:\particle\luminous-particle>cd \particle\firmware\main 

C:\Particle\firmware\main>make all PLATFORM=photon program-dfu APPDIR=../../luminous-particle  
make -C ../modules/photon/user-part all program-dfu
make[1]: Entering directory `C:/Particle/firmware/modules/photon/user-part'
make -C ../../../user 
make[2]: Entering directory `C:/Particle/firmware/user'
Building cpp file: ../../luminous-particle//src/compositemodule.cpp
Invoking: ARM GCC CPP Compiler
mkdir -p ../build/target/user/platform-6-m/luminous-particle/src/
arm-none-eabi-gcc -DSTM32_DEVICE -DSTM32F2XX -DPLATFORM_THREADING=1 -DPLATFORM_ID=6 -DPLATFORM_NAME=photon -DUSBD_VID_SPARK=0x2B04 -DUSBD_PID_DFU=0xD006 -DUSBD_PID_CDC=0xC006 -DSPARK_PLATFORM -g3 -gdwarf-2 -Os -mcpu=cortex-m3 -mthumb -DINCLUDE_PLATFORM=1 -DPRODUCT_ID=6 -DPRODUCT_FIRMWARE_VERSION=65535 -DUSE_STDPERIPH_DRIVER -DDFU_BUILD_ENABLE -DSYSTEM_VERSION_STRING=0.6.2 -DRELEASE_BUILD -I./inc -I../wiring/inc -I../system/inc -I../services/inc -I../communication/src -I../hal/inc -I../hal/shared -I../hal/src/photon -I../hal/src/stm32f2xx -I../hal/src/stm32 -I../hal/src/photon/api -I../platform/shared/inc -I../platform/MCU/STM32F2xx/CMSIS/Include -I../platform/MCU/STM32F2xx/CMSIS/Device/ST/Include -I../platform/MCU/STM32F2xx/SPARK_Firmware_Driver/inc -I../platform/MCU/shared/STM32/inc -I../platform/MCU/STM32F2xx/STM32_StdPeriph_Driver/inc -I../platform/MCU/STM32F2xx/STM32_USB_Device_Driver/inc -I../platform/MCU/STM32F2xx/STM32_USB_Host_Driver/inc -I../platform/MCU/STM32F2xx/STM32_USB_OTG_Driver/inc -I../dynalib/inc -I../../luminous-particle//src -I./libraries -I../../luminous-particle/lib/Adafruit-GFX-Library/src -I../../luminous-particle/lib/Adafruit_SSD1351_Photon/src -I../../luminous-particle/lib/clickButton/src -I../../luminous-particle/lib/PCA9685-Particle/src -I. -MD -MP -MF ../build/target/user/platform-6-m/luminous-particle/src/compositemodule.o.d -ffunction-sections -fdata-sections -Wall -Wno-switch -Wno-error=deprecated-declarations -fmessage-length=0 -fno-strict-aliasing -DSPARK=1 -DPARTICLE=1 -Wundef -DSTART_DFU_FLASHER_SERIAL_SPEED=14400 -DSTART_YMODEM_FLASHER_SERIAL_SPEED=28800 -DSPARK_PLATFORM_NET=BCM9WCDUSI09 -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc  -DLOG_INCLUDE_SOURCE_INFO=1 -DPARTICLE_USER_MODULE -DUSER_FIRMWARE_IMAGE_SIZE=0x20000 -DUSER_FIRMWARE_IMAGE_LOCATION=0x80A0000 -DMODULAR_FIRMWARE=1 -DMODULE_VERSION=4 -DMODULE_FUNCTION=5 -DMODULE_INDEX=1 -DMODULE_DEPENDENCY=4,2,108 -D_WINSOCK_H -D_GNU_SOURCE -DLOG_MODULE_CATEGORY="\"app\""  -fno-exceptions -fno-rtti -fcheck-new -std=gnu++11 -c -o ../build/target/user/platform-6-m/luminous-particle/src/compositemodule.o ../../luminous-particle//src/compositemodule.cpp

Building target: ../build/target/user/platform-6-m/luminous-particle/libuser.a
Invoking: ARM GCC Archiver
mkdir -p ../build/target/user/platform-6-m/luminous-particle/
arm-none-eabi-gcc-ar -cr ../build/target/user/platform-6-m/luminous-particle/libuser.a ../build/target/user/platform-6-m/luminous-particle/Adafruit-GFX-Library/src/glcdfont.o ../build/target/user/platform-6-m/luminous-particle/Adafruit-GFX-Library/src/fontconvert/fontconvert.o ../build/target/user/platform-6-m/luminous-particle/src/compositemodule.o ../build/target/user/platform-6-m/luminous-particle/src/debug.o ../build/target/user/platform-6-m/luminous-particle/src/effects.o ../build/target/user/platform-6-m/luminous-particle/src/emitter.o ../build/target/user/platform-6-m/luminous-particle/src/hsicolor.o ../build/target/user/platform-6-m/luminous-particle/src/hsilight.o ../build/target/user/platform-6-m/luminous-particle/src/luminous-particle.o ../build/target/user/platform-6-m/luminous-particle/src/outputpca9685.o ../build/target/user/platform-6-m/luminous-particle/Adafruit-GFX-Library/src/Adafruit_GFX.o ../build/target/user/platform-6-m/luminous-particle/Adafruit_SSD1351_Photon/src/Adafruit_SSD1351_Photon.o ../build/target/user/platform-6-m/luminous-particle/clickButton/src/clickButton.o ../build/target/user/platform-6-m/luminous-particle/PCA9685-Particle/src/PCA9685.o

make[2]: Leaving directory `C:/Particle/firmware/user'
make -C ../../../hal-dynalib 
make[2]: Entering directory `C:/Particle/firmware/hal-dynalib'
make[2]: Nothing to be done for `all'.
make[2]: Leaving directory `C:/Particle/firmware/hal-dynalib'
make -C ../../../services-dynalib 
make[2]: Entering directory `C:/Particle/firmware/services-dynalib'
make[2]: Nothing to be done for `all'.
make[2]: Leaving directory `C:/Particle/firmware/services-dynalib'
make -C ../../../system-dynalib 
make[2]: Entering directory `C:/Particle/firmware/system-dynalib'
make[2]: Nothing to be done for `all'.
make[2]: Leaving directory `C:/Particle/firmware/system-dynalib'
make -C ../../../rt-dynalib 
make[2]: Entering directory `C:/Particle/firmware/rt-dynalib'
make[2]: Nothing to be done for `all'.
make[2]: Leaving directory `C:/Particle/firmware/rt-dynalib'
make -C ../../../wiring 
make[2]: Entering directory `C:/Particle/firmware/wiring'
make[2]: Nothing to be done for `all'.
make[2]: Leaving directory `C:/Particle/firmware/wiring'
make -C ../../../communication-dynalib 
make[2]: Entering directory `C:/Particle/firmware/communication-dynalib'
make[2]: Nothing to be done for `all'.
make[2]: Leaving directory `C:/Particle/firmware/communication-dynalib'
make -C ../../../platform 
make[2]: Entering directory `C:/Particle/firmware/platform'
make[2]: Nothing to be done for `all'.
make[2]: Leaving directory `C:/Particle/firmware/platform'
make -C ../../../wiring_globals 
make[2]: Entering directory `C:/Particle/firmware/wiring_globals'
make[2]: Nothing to be done for `all'.
make[2]: Leaving directory `C:/Particle/firmware/wiring_globals'
Building target: ../../luminous-particle/target/luminous-particle.elf
Invoking: ARM GCC C++ Linker
mkdir -p ../../luminous-particle/target/
arm-none-eabi-g++ -DSTM32_DEVICE -DSTM32F2XX -DPLATFORM_THREADING=1 -DPLATFORM_ID=6 -DPLATFORM_NAME=photon -DUSBD_VID_SPARK=0x2B04 -DUSBD_PID_DFU=0xD006 -DUSBD_PID_CDC=0xC006 -g3 -gdwarf-2 -Os -mcpu=cortex-m3 -mthumb -DINCLUDE_PLATFORM=1 -fno-builtin -DUSE_STDPERIPH_DRIVER -DDFU_BUILD_ENABLE -DSYSTEM_VERSION_STRING=0.6.2 -DRELEASE_BUILD -Werror -I./inc -I../../../user/inc -I../../../dynalib/inc -I../../../services/inc -I../../../hal/inc -I../../../hal/shared -I../../../hal/src/photon -I../../../hal/src/stm32f2xx -I../../../hal/src/stm32 -I../../../hal/src/photon/api -I../../../system/inc -I../../../rt-dynalib/inc -I../../../wiring/inc -I../../../modules/photon/system-part1/inc -I../../../modules/shared/stm32f2xx/inc -I../../../platform/shared/inc -I../../../platform/MCU/STM32F2xx/CMSIS/Include -I../../../platform/MCU/STM32F2xx/CMSIS/Device/ST/Include -I../../../platform/MCU/STM32F2xx/SPARK_Firmware_Driver/inc -I../../../platform/MCU/shared/STM32/inc -I../../../platform/MCU/STM32F2xx/STM32_StdPeriph_Driver/inc -I../../../platform/MCU/STM32F2xx/STM32_USB_Device_Driver/inc -I../../../platform/MCU/STM32F2xx/STM32_USB_Host_Driver/inc -I../../../platform/MCU/STM32F2xx/STM32_USB_OTG_Driver/inc -I. -MD -MP -MF ../../luminous-particle/target/luminous-particle.elf.d -ffunction-sections -fdata-sections -Wall -Wno-switch -Wno-error=deprecated-declarations -fmessage-length=0 -fno-strict-aliasing -DSPARK=1 -DPARTICLE=1 -Wundef -DSTART_DFU_FLASHER_SERIAL_SPEED=14400 -DSTART_YMODEM_FLASHER_SERIAL_SPEED=28800 -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc -DUSER_FIRMWARE_IMAGE_SIZE=0x20000 -DUSER_FIRMWARE_IMAGE_LOCATION=0x80A0000 -DMODULAR_FIRMWARE=1 -DMODULE_VERSION=4 -DMODULE_FUNCTION=5 -DMODULE_INDEX=1 -DMODULE_DEPENDENCY=4,2,108 -D_WINSOCK_H -D_GNU_SOURCE ../../luminous-particle/target//obj/src/module_info.o ../../luminous-particle/target//obj/src/user_export.o ../../luminous-particle/target//obj/src/user_module.o ../../luminous-particle/target//obj/src/newlib_stubs.o  --output ../../luminous-particle/target/luminous-particle.elf -Wl,--whole-archive ../../../hal/src/photon/lib/STM32F2xx_Peripheral_Libraries.a -Wl,--no-whole-archive -nostartfiles -Xlinker --gc-sections -L../../../build/arm/linker/stm32f2xx  -L../../../build/target/user/platform-6-m/luminous-particle/ -L../../../build/target/services-dynalib/arm/ -L../../../build/target/hal-dynalib/platform-6-m/ -L../../../build/target/system-dynalib/platform-6-m/ -L../../../build/target/rt-dynalib/platform-6-m/ -L../../../build/target/wiring/platform-6-m/ -L../../../build/target/communication-dynalib/platform-6-m/ -L../../../build/target/platform/platform-6-m/ -L../../../build/target/wiring_globals/platform-6-m/ -L../../../hal/src/photon/lib/ -L../../../build/arm/linker -Wl,--whole-archive -luser -lhal-dynalib -lservices-dynalib -lsystem-dynalib -lrt-dynalib -lwiring -lcommunication-dynalib -lplatform -lwiring_globals -Wl,--no-whole-archive -lnosys --specs=nano.specs -L../../../modules/photon/system-part2 -L../../../modules/photon/system-part1 -L. -T./linker.ld -Wl,--defsym,USER_FIRMWARE_IMAGE_SIZE=0x20000 -Wl,--defsym,USER_FIRMWARE_IMAGE_LOCATION=0x80A0000 -Wl,-Map,../../luminous-particle/target/luminous-particle.map

Invoking: ARM GNU Create Flash Image
arm-none-eabi-objcopy -O binary ../../luminous-particle/target/luminous-particle.elf ../../luminous-particle/target/luminous-particle.bin.pre_crc
if [ -s ../../luminous-particle/target/luminous-particle.bin.pre_crc ]; then \
	head -c $((`stat --print %s ../../luminous-particle/target/luminous-particle.bin.pre_crc` - 38)) ../../luminous-particle/target/luminous-particle.bin.pre_crc > ../../luminous-particle/target/luminous-particle.bin.no_crc && \
	tail -c 38 ../../luminous-particle/target/luminous-particle.bin.pre_crc > ../../luminous-particle/target/luminous-particle.bin.crc_block && \
	test "0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f20280078563412" = `xxd -p -c 500 ../../luminous-particle/target/luminous-particle.bin.crc_block` && \
	shasum -a 256 ../../luminous-particle/target/luminous-particle.bin.no_crc | cut -c 1-65 | xxd -r -p | dd bs=1 of=../../luminous-particle/target/luminous-particle.bin.pre_crc seek=$((`stat --print %s ../../luminous-particle/target/luminous-particle.bin.pre_crc` - 38)) conv=notrunc  && \
	head -c $((`stat --print %s ../../luminous-particle/target/luminous-particle.bin.pre_crc` - 4)) ../../luminous-particle/target/luminous-particle.bin.pre_crc > ../../luminous-particle/target/luminous-particle.bin.no_crc && \
	 crc32 ../../luminous-particle/target/luminous-particle.bin.no_crc | cut -c 1-10 | xxd -r -p | dd bs=1 of=../../luminous-particle/target/luminous-particle.bin.pre_crc seek=$((`stat --print %s ../../luminous-particle/target/luminous-particle.bin.pre_crc` - 4)) conv=notrunc ;\
	fi
[ ! -f ../../luminous-particle/target/luminous-particle.bin ] || rm ../../luminous-particle/target/luminous-particle.bin
mv ../../luminous-particle/target/luminous-particle.bin.pre_crc ../../luminous-particle/target/luminous-particle.bin

arm-none-eabi-objcopy -O ihex ../../luminous-particle/target/luminous-particle.elf ../../luminous-particle/target/luminous-particle.hex
arm-none-eabi-size --format=berkeley ../../luminous-particle/target/luminous-particle.elf
   text	   data	    bss	    dec	    hex	filename
  32420	    196	   1984	  34600	   8728	../../luminous-particle/target/luminous-particle.elf
arm-none-eabi-objdump -h -S ../../luminous-particle/target/luminous-particle.elf > ../../luminous-particle/target/luminous-particle.lst
dfu-suffix -v 2B04 -p D006 -a ../../luminous-particle/target/luminous-particle.dfu
dfu-suffix (dfu-util) 0.8

Copyright 2011-2012 Stefan Schmidt, 2013-2014 Tormod Volden
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to dfu-util@lists.gnumonks.org

Suffix successfully added to file
Serial device PARTICLE_SERIAL_DEV : not available
Flashing using dfu:
dfu-util -d 0x2B04:0xD006 -a 0 -s 0x80A0000:leave -D ../../luminous-particle/target/luminous-particle.dfu
dfu-util 0.8

Copyright 2005-2009 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2014 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to dfu-util@lists.gnumonks.org

Deducing device DFU version from functional descriptor length
Deducing device DFU version from functional descriptor length
Opening DFU capable USB device...
ID 2b04:d006
Run-time device DFU version 011a
Claiming USB DFU Interface...
Setting Alternate Setting #0 ...
Determining device status: state = dfuIDLE, status = 0
dfuIDLE, continuing
DFU mode device DFU version 011a
Device returned transfer size 4096
DfuSe interface name: "Internal Flash   "
Downloading to address = 0x080a0000, size = 32616

Download	[                         ]   0%            0 bytes
Download	[                         ]   0%            0 bytes
Download	[===                      ]  12%         4096 bytes
Download	[======                   ]  25%         8192 bytes
Download	[=========                ]  37%        12288 bytes
Download	[============             ]  50%        16384 bytes
Download	[===============          ]  62%        20480 bytes
Download	[==================       ]  75%        24576 bytes
Download	[=====================    ]  87%        28672 bytes
Download	[=========================] 100%        32616 bytes
Download done.
File downloaded successfully
make[1]: Leaving directory `C:/Particle/firmware/modules/photon/user-part'
make: Nothing to be done for `program-dfu'.

C:\Particle\firmware\main>cd \particle\luminous-particle 
