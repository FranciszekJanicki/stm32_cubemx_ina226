PROJECT_DIR := $(shell pwd)
APP_DIR := $(PROJECT_DIR)/app
DRIVERS_DIR := $(PROJECT_DIR)/Drivers
CORE_DIR := $(PROJECT_DIR)/Core
CMAKE_DIR := $(PROJECT_DIR)/cmake
BUILD_DIR := $(PROJECT_DIR)/build
UTILITY_DIR := $(APP_DIR)/utility
MAIN_DIR := $(APP_DIR)/main

PROJECT_NAME := project
PROJECT_BINARY := $(BUILD_DIR)/app/main/main.elf

UART_PORT := /dev/ttyUSB0
USB_PORT := /dev/ttyACM0