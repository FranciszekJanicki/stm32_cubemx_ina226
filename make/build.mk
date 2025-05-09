include make/common.mk

.PHONY: build
build: 
	cd $(BUILD_DIR) && $(MAKE)

.PHONY: clean
clean: 
	cd $(PROJECT_DIR) && rm -rf $(BUILD_DIR)

.PHONY: cmake
cmake:
	cd $(PROJECT_DIR) && $(MAKE) clean && mkdir build && cmake -S . -B build
