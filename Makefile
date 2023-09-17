CC = gcc
CFLAGS =  -Wall
TARGET = blockchain
SOURCE = blockchain.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)