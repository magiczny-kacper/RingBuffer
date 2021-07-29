################################################################################
#
# Script for compiling Unity test framework on PC. As a result static library
# libunity.a is created.
#
################################################################################

include ../../../makefiles/toolchain_pc.mk
include ../../../makefiles/platform.mk

################################################################################
# File paths defines

C_EXT := c
C_SRC := $(wildcard $(addsuffix /*.$(C_EXT), $(SRC_DIRS)))
OBJS := $(addprefix $(OUT_DIR), $(patsubst %.c,%.o,$(notdir $(C_SRC))))
DEPS := $(OBJS:.o=.d)

VPATH := $(sort $(INC_DIRS) $(SRC_DIRS))

#
################################################################################

################################################################################
# Compiler and linker defines

C_FLAGS := -MMD
INC_FLAGS := $(patsubst %, -I%, $(INC_DIRS))
C_FLAGS += $(INC_FLAGS)

#
################################################################################

################################################################################
# Targets

all: print_srcs out_dir $(OUT_DIR)lib/lib$(TARGET_NAME).a

$(OUT_DIR)lib/lib$(TARGET_NAME).a : $(OBJS)
	$(ECHO) 'Making static lib: $@'
	$(AR) -rcs $@ $^
	$(ECHO) ' '

$(OUT_DIR)%.o : %.$(C_EXT)
	$(ECHO) 'Compiling file: $<'
	$(CC) -c $(C_FLAGS) $< -o $@
	$(ECHO) ' '

# header dependencies
-include $(DEPS)

print_srcs :
	$(ECHO) 'C sources:'
	$(ECHO) $(C_SRC)
	$(ECHO) 'OBJ files:'
	$(ECHO) $(OBJS)
	$(ECHO) ' '
	
out_dir :
	$(MKDIR) $(subst /,,$(OUT_DIR))
	$(MKDIR) $(OUT_DIR)lib

clean :
	$(RM) $(subst /,,$(OUT_DIR))
	$(RM) $(OUT_DIR)lib