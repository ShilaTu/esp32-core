
.PHONY: default
default: build tests

.PHONY: help
help:
	@echo "--- standard targets ---"
	@echo "make help      | shows this help"
	@echo "make default   | starts build and tests"
	@echo "make setup     | does nothing"
	@echo "make all       | starts all builds and tests"
	@echo "make clean     | cleans build files"
	@echo "make distclean | removes everything generated"
	@echo
	@echo "--- specific targets ---"
	@echo "make build     | just builds "
	@echo "make tests     | just tests "

.PHONY: all
all:
	@true

.PHONY: setup
setup:
	@true

.PHONY: clean
clean:
	@true

.PHONY: distclean
distclean:
	@true

.PHONY: build
build:
	@true

.PHONY: tests
tests:
	@true
