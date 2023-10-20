esptool --port COM23 --baud 460800 write_flash --flash_mode dio --flash_freq 40m --flash_size 4MB 0x10000 firmware.bin
    