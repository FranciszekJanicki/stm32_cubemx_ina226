include make/common.mk

.PHONY: cubemx
cubemx: 
	stm32cubemx

.PHONY: cubeprog
cubeprog:
	/opt/stm32cubeprog/bin/STM32CubeProgrammer
