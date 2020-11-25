# お決まり
.RECIPEPREFIX = >
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
.DELETE_ON_ERROR:
MAKEFLAGS += -r

# ここからどうぞ
TARGET := elf_loader
CXX := g++
# O2つけると動かないので注意
CXXFLAGS := -Wall -Wextra --pedantic-errors -g -std=c++17 -static -T linker.ld

CXXSUFFIX := .cpp .hpp

CSRCS := $(wildcard *$(CSUFFIX))
CXXSRCS := $(wildcard *$(CXXSUFFIX))

.PHONY: $(TARGET)
$(TARGET): $(CXXSRCS)
> $(CXX) $(CXXFLAGS) $^ -o ../build/$@

.PHONY: clean
clean:
> rm -f $(TARGET)
