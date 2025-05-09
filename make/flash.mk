include make/common.mk

.PHONY: flash_uart
flash_uart: 
	STM32_Programmer_CLI -c port=swd -d $(PROJECT_BINARY) -rst

.PHONY: flash_usb
flash_usb:
	STM32_Programmer_CLI -c port=$(USB_PORT) -d $(PROJECT_BINARY) -rst