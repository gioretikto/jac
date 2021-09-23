CC = gcc

CFLAGS = -Wall -Wextra -ggdb3

ifeq ($(SYSTEM), SunOS)
	CFLAGS+= -D__EXTENSIONS__
endif

LDFLAGS = -lm
OBJFILES = main.o functions.o parser.o lists.o syntax.o
TARGET = jac

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES)
install:
	cp $(TARGET) /usr/local/bin
