# お決まり
.RECIPEPREFIX = >
.SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
.DELETE_ON_ERROR:
MAKEFLAGS += -r

# ここからどうぞ
.PHONY: default
default: chflg.c
> gcc chflg.c -o chflg
> ./chflg elf_loader

.PHONY: clean
clean:
> rm -f elf_loader chflg
