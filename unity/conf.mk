################################################################################
# File:     conf.mk
# Author:   GAndaLF
# Brief:    Makefile configuration script for unity target.
#
# This file contains configuration variables for unity target.
# After setting these variables, actual build script is called.
################################################################################

TARGET_NAME := unity

# Output folder containing object files
OUT_DIR := out/

# Paths containing headers (.h)
INC_DIRS := \
core \
fixture \

# Paths containing source (.c)
SRC_DIRS := \
core \
fixture \

include build.mk