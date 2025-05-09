include make/common.mk

.PHONY: add_utility
add_utility:
	git submodule add -f https://github.com/franciszekjanicki/utility.git $(UTILITY_DIR)

.PHONY: remove_utility
remove_utility:
	git submodule deinit -f $(UTILITY_DIR)
	git rm -rf $(UTILITY_DIR)
	rm -rf $(UTILITY_DIR)
	rm -rf .git/modules/app/utility

.PHONY: add_third_party
add_third_party: add_utility

.PHONY: remove_third_party
remove_third_party: remove_utility
