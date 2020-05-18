CC = g++
CFLAGS = -Wall
LDFLAGS = -lboost_system -Iinclude
OBJFILES_c = battleship.o client.o
OBJFILES_s = battleship.o server.o
TARGET_c = client
TARGET_s = server

all: $(TARGET_c) $(TARGET_s)
$(TARGET_c): $(OBJFILES_c)
	$(CC) $(CFLAGS) -o $(TARGET_c) $(OBJFILES_c) $(LDFLAGS)
$(TARGET_s): $(OBJFILES_s)
	$(CC) $(CFLAGS) -o $(TARGET_s) $(OBJFILES_s) $(LDFLAGS)
clean:
	rm -f $(OBJFILES_c) $(OBJFILES_s) $(TARGET_c) $(TARGET_s)