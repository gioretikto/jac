CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lm
OBJFILES = jac.o functions.o
TARGET = jac

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES)
install:
	cp $(TARGET) /usr/local/bin
