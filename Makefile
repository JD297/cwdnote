PREFIX		= /usr/local
BINDIR		= $(PREFIX)/bin
MANDIR		= $(PREFIX)/share/man

TARGET		= cwdnote
SRC		= src
SRC_FILES	= $(wildcard $(SRC)/*.c)
OBJ_FILES	= $(patsubst $(SRC)/%.c,$(SRC)/%.o,$(SRC_FILES))

CC		= gcc
CCFLAGS		= -Wall -Wextra -Wpedantic
CCFLAGSPROG	= -DTARGET=\"$(TARGET)\"
CCLIBS		=
#CCFLAGSDEBUG	= -g
#CCLIBSSTATIC	= -static

$(TARGET): $(OBJ_FILES)
	$(CC) $(CCFLAGS) $(CCFLAGSDEBUG) $(OBJ_FILES) $(CCLIBSSTATIC) $(CCLIBS) -o $(TARGET)

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CCFLAGS) $(CCFLAGSPROG) -c -o $@ $<

clean:
	rm -f $(SRC)/*.o $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	rm -f $(BINDIR)/$(TARGET)
