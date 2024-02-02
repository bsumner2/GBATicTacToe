# Written by Burton Sumner with help from Tonc Library (https://www.coronac.com/tonc/text/setup.htm)


# If project name not explicitly specified during make command execution, use default name, "main."
ifeq '$(PROJ_NAME)' ''
	PROJ_NAME=TicTacToe
endif

#-------------------------------Project Files and Dirs-----------------------------------------------------

TARGET=$(PROJ_NAME)
SRC=./src
BIN=./bin
INC=./include

C_OBJS=$(shell find $(SRC) -type f -iname '*.c' | sed 's-\./src-\./bin-g' | sed 's/\.c/\.o/g')
CXX_OBJS=$(shell find $(SRC) -type f -iname '*.cpp' | sed 's-\./src-\./bin-g' | sed 's/\.cpp/\.o/g')

#-------------------------------Build Definitions----------------------------------------------------------
PREFIX=arm-none-eabi-
CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
LD=$(PREFIX)g++
OBJ_CPY=$(PREFIX)objcopy

ARCH=-mthumb-interwork -mthumb
SPECS=-specs=gba.specs

CFLAGS=$(ARCH) -O2 -Wall -fno-strict-aliasing -I$(INC)
LDFLAGS=$(ARCH) $(SPECS)

.PHONY: build clean

#-------------------------------Build----------------------------------------------------------------------

.SILENT:

test: clean build
	mgba-qt $(BIN)/$(TARGET).gba &

build: ./bin $(TARGET).gba

./bin:
	mkdir -p ./bin

$(TARGET).gba: $(TARGET).elf
	$(OBJ_CPY) -v -O binary $(BIN)/$< $(BIN)/$@
	-@gbafix $(BIN)/$@

$(TARGET).elf: $(C_OBJS) $(CXX_OBJS)
	$(LD) $^ $(LDFLAGS) -o $(BIN)/$@

$(CXX_OBJS): $(BIN)/%.o : $(SRC)/%.cpp
	$(CXX) -c $(CFLAGS) -o $@

$(C_OBJS): $(BIN)/%.o : $(SRC)/%.c
	$(CC) -c $< $(CFLAGS) -o $@

#-------------------------------Build----------------------------------------------------------------------
clean: 
	@rm -fv $(BIN)/*.elf
	@rm -fv $(BIN)/*.o
	@rm -fv $(BIN)/*.gba
