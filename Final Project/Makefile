########################################################################
# Makefile settings - Can be customized.
LIBS = lib_projd.a
RELEASE_OPTIMIZATION = Os
USER_SETTINGS = -I ./ -DYOURNAME="DaVonte J Carter Vault"

########################################################################
# DO NOT CHANGE ANYTHING BELOW THIS LINE
########################################################################
CC = avr-gcc
CLOCK_RATE=16000000UL
TARGET_SETTINGS = -mmcu=atmega328p -DF_CPU=16000000UL
EXT = .c
SRCDIR = .
SRC = $(wildcard $(SRCDIR)/*$(EXT))
CXXFLAGS = -Wall $(TARGET_SETTINGS) $(USER_SETTINGS)
LDFLAGS =
DEBUGOBJ = $(SRC:$(SRCDIR)/%$(EXT)=./bin/Debug/%.o)
RELEASEOBJ = $(SRC:$(SRCDIR)/%$(EXT)=./bin/Release/%.o)

# Build the app for debugging
debug: bin/Debug/.exists dclean $(DEBUGOBJ)
	$(CC) $(CXXFLAGS) -o bin/Debug/$(APPNAME).elf $(DEBUGOBJ) $(LIBS) $(LDFLAGS)
	avr-objdump -h -S bin/Debug/$(APPNAME).elf > bin/Debug/$(APPNAME).lss
# Build the application for running on the real hardware.
release: bin/Release/.exists rclean $(RELEASEOBJ)
	$(CC) $(CXXFLAGS) -o bin/Release/$(APPNAME).elf $(RELEASEOBJ) $(LIBS) $(LDFLAGS)
	avr-objdump -h -S bin/Release/$(APPNAME).elf > bin/Release/$(APPNAME).lss
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex bin/Release/$(APPNAME).elf bin/Release/$(APPNAME).hex
	avr-size bin/Release/$(APPNAME).elf
# Clean the project folders
clean:
	-@rm -rf bin/Debug/*.*
	-@rm -rf bin/Release/*.*
	
# compile rules for either release or debug
bin/Debug/%.o: $(SRCDIR)/%$(EXT)
	$(CC) -O0 -g $(CXXFLAGS) -o $@ -c $<
bin/Release/%.o: $(SRCDIR)/%$(EXT)
	$(CC) -$(RELEASE_OPTIMIZATION) $(CXXFLAGS) -o $@ -c $<

# cleaning rules for release and debug
.PHONY dclean:
dclean:
	-@rm -f bin/Debug/*.o
	-@rm -f bin/Debug/$(APPNAME).*
.PHONY rclean:
rclean:
	-@rm -f bin/Release/*.o
	-@rm -f bin/Release/$(APPNAME).*

# create the Release folder if it doesn't exist
bin/Release/.exists:
	-mkdir -p bin/Release
	-touch $@
# create the Debug folder if it doesn't exist
bin/Debug/.exists:
	mkdir -p bin/Debug
	touch $@
