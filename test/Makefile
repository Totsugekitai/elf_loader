# お決まり
.RECIPEPREFIX = >
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
.DELETE_ON_ERROR:
MAKEFLAGS += -r

# ここからどうぞ
CC := gcc
CFLAGS := -Wall -Wextra -std=c11
MYLD := ../build/elf_loader

.PHONY: default
default:
> make test-static

.PHONY: test0s
test0s: test0s.c
> $(CC) $(CFLAGS) -static $^ -o $@
> $(MYLD) ./$@

.PHONY: test1s
test1s: test1s.c
> $(CC) $(CFLAGS) -static $^ -o $@
> $(MYLD) ./$@ abc def ghi

.PHONY: test-static
test-static:
> make test0s
> make test1s

.PHONY: clean
clean:
> rm -f test0s
