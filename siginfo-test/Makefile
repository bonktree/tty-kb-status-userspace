LDFLAGS = -static -Os -s

all: siginfo

siginfo: siginfo.o main.o
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm *.o siginfo

