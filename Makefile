include make/build.mk
include make/clang.mk
include make/cubemx.mk
include make/flash.mk
include make/monitor.mk
include make/third_party.mk

.PHONY: all
all:
	$(MAKE) clang_format && $(MAKE) build && $(MAKE) flash_uart && $(MAKE) monitor_uart
