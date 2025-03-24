
all:




%.o: %.c
	$(CC) -o $@ -c $<


sslreq: ssl_req.o hexdump.o
	$(CC) -o $@ $^ -lssl -lcrypto


all: sslreq
