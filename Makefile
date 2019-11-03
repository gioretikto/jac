CC = gcc
CFLAGS = -Wall -pedantic -Wextra -std=c99
LDFLAGS = -lm
OBJFILES = jac.o
TARGET = jac
all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET)
install:
	cp $(TARGET) /usr/local/bin