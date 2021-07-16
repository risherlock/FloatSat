# Rishav (2020)

######################################
# target
######################################
TARGET = main

######################################
# building variables
######################################
# debug build?
DEBUG = 1

# optimization
OPT = \
-O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \

# ASM sources
ASM_SOURCES =

# C++ sources
CXX_SOURCES = \
hello_motor.cpp

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
CXX = $(GCC_PATH)/arm-none-eabi-g++
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
CXX = arm-none-eabi-g++
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=soft

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_STM32_DISCOVERY \
-DSTM32F40_41xxx \

# AS includes
AS_INCLUDES = \

# C includes
C_INCLUDES =  \
-I"../dependencies/rodos/src/bare-metal/stm32f4/CMSIS/Device/ST/STM32F4xx/Include" \
-I"../dependencies/rodos/src/bare-metal/stm32f4/STM32F4xx_StdPeriph_Driver/inc"  \
-I"../dependencies/rodos/src/bare-metal/stm32f4/platform-parameter/discovery" \
-I"../dependencies/rodos/src/bare-metal/stm32f4/CMSIS/Include" \
-I"../dependencies/rodos/src/bare-metal/stm32f4/sdCard" \
-I"../dependencies/rodos/src/bare-metal/stm32f4/hal" \
-I"../dependencies/rodos/src/bare-metal/stm32f4" \
-I"../dependencies/rodos/src/bare-metal-generic" \
-I"../dependencies/rodos/src/independent/gateway" \
-I"../dependencies/rodos/src/independent" \
-I"../dependencies/rodos/api/hal" \
-I"../dependencies/rodos/api" \
-I"../dependencies/rodos/default_usr_configs" \
-I"../dependencies/orion_core/inc"

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# CXXFLAGS
#######################################
CXXFLAGS=$(CFLAGS)
CXXFLAGS+=-fno-rtti -fno-exceptions
CXXFLAGS+=-std=c++11
CXXFLAGS+=-U__STRICT_ANSI__

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = "..\dependencies\rodos\src\bare-metal\stm32f4\scripts\stm32_flash.ld" 

# libraries
LIBS = -lm -lrodos
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) -nostartfiles -nodefaultlibs -nostdlib -Xlinker --gc-sections \
$(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
LIBDIR = \
-L"..\dependencies\rodos\build"

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
# list of C++ objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CXX_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CXX_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# flash
#######################################
flash: all
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(BUILD_DIR)/$(TARGET).hex verify reset exit"

#######################################
# clean up
#######################################
clean:
	del $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
