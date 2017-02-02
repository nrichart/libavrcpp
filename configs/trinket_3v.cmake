# Trinket 3.3V Configuration
set(LIBAVRC++_NAME                      "Adafruit Trinket 8MHz")
set(LIBAVRC++_UPLOAD_PROTOCOL           "usbtiny")
set(LIBAVRC++_UPLOAD_MAXIMUM_SIZE       "5310")
set(LIBAVRC++_BOOTLOADER_LOW_FUSES      "0xf1")
set(LIBAVRC++_BOOTLOADER_HIGH_FUSES     "0xd5")
set(LIBAVRC++_BOOTLOADER_EXTENDED_FUSES "0xFE")
set(LIBAVRC++_BUILD_MCU                 "attiny85")
set(LIBAVRC++_BUILD_F_CPU               "8000000L")
set(LIBAVRC++_BUILD_CORE                "arduino:arduino")
set(LIBAVRC++_BUILD_VARIANT             "tiny8")
set(LIBAVRC++_UPLOAD_DEFAULT_PORT       "/dev/ttyACM0")
#
#trinket3.name=Adafruit Trinket 8MHz
#trinket3.bootloader.low_fuses=0xF1
#trinket3.bootloader.high_fuses=0xD5
#trinket3.bootloader.extended_fuses=0xFE
#trinket3.bootloader.tool=arduino:avrdude
#trinket3.build.mcu=attiny85
#trinket3.build.f_cpu=8000000L
#trinket3.build.core=arduino:arduino
#trinket3.build.variant=tiny8
#trinket3.build.board=AVR_TRINKET3
#trinket3.upload.tool=arduino:avrdude
#trinket3.upload.maximum_size=5310

##############################################################
# Trinket 5V Configuration
#trinket5.name=Adafruit Trinket 16MHz
#trinket5.bootloader.low_fuses=0xF1
#trinket5.bootloader.high_fuses=0xD5
#trinket5.bootloader.extended_fuses=0xFE
#trinket5.bootloader.tool=arduino:avrdude
#trinket5.build.mcu=attiny85
#trinket5.build.f_cpu=16000000L
#trinket5.build.core=arduino:arduino
#trinket5.build.variant=tiny8
#trinket5.build.board=AVR_TRINKET5
#trinket5.upload.tool=arduino:avrdude
#trinket5.upload.maximum_size=5310
