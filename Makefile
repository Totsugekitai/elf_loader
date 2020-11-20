# お決まり
.RECIPEPREFIX = >
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
.DELETE_ON_ERROR:
MAKEFLAGS += -r

# ここからどうぞ
.PHONY: default
default:
> make -C src/

.PHONY: test
test:
> make -s -C build/
> make -s -C test/

.PHONY: clean
clean:
> make clean -C src/
> make clean -C test/

.PHONY: build-clean
build-clean:
> make clean -C build/
