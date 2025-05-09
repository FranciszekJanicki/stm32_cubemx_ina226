include make/common.mk

.PHONY: clang_tidy
clang_tidy:
	for ext in h c cpp hpp; do
		find $(APP_DIR) -iname "*.$$ext" -print0 | xargs -0 -r clang-tidy -i; \
	done

.PHONY: clang_format
clang_format:
	for ext in h c cpp hpp; do \
		find $(APP_DIR) -iname "*.$$ext" -print0 | xargs -0 -r clang-format -i; \
	done
