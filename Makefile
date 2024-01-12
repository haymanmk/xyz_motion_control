#  Part of Grbl
#
#  Copyright (c) 2009-2011 Simen Svale Skogsrud
#  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC
#
#  Grbl is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Grbl is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.


# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEBUG		= 1

# Target
TARGET		= xyz_motion_control

DEVICE		?= stm32
CLOCK		= 16000000
CC			= gcc
CFLAG		= -DF_CPU=${CLOCK}
SOURCE		= main.c gcode.c motion_control.c nuts_bolts.c planner.c protocol.c report.c \
            stepper.c settings.c system.c \
			 
BUILDDIR	= build
SOURCEDIR	= src

# Libraries
LIBS		= -lm
LDFLAG		= $(LIBS) -Wl,-Map=$(BUILDDIR)/$(TARGET).map,--cref -Wl,--gc-sections


ifeq ($(DEBUG), 1)
CFLAG += -g -Wall -O0 
LDFLAG += -v
endif

OBJECTS = $(addprefix $(BUILDDIR)/,$(notdir $(SOURCE:.c=.o)))


# symbolic targets:
all:	$(TARGET).hex

$(TARGET).hex: $(BUILDDIR)/main.elf
	rm -f $(TARGET).hex

# file targets:
$(BUILDDIR)/main.elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAG) -o $(BUILDDIR)/main.elf

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAG) -MMD -MP -c $< -o $@

$(BUILDDIR):
	mkdir $@


cpp:
	$(CC) -E $(SOURCEDIR)/main.c

clean:
	rm -f $(TARGET).hex $(BUILDDIR)/*.o $(BUILDDIR)/*.d $(BUILDDIR)/*.elf


# include generated header dependencies
-include $(BUILDDIR)/$(OBJECTS:.o=.d)
