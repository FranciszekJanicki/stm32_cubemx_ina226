include make/common.mk

.PHONY: monitor_uart
monitor_uart:
	minicom -D $(UART_PORT) -b 115200

.PHONY: monitor_usb
monitor_usb:
	minicom -D $(USB_PORT) -b 115200