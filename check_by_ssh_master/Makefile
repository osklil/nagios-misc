CC = gcc
CFLAGS = -Wall -std=gnu99 -I.
LDADD =

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

check_by_ssh_master: main.o
	$(CC) -o $@ $^ $(LDADD)

clean:
	rm -f *.o check_by_ssh_master
