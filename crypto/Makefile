CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -g -I.
LLIBS = -lssl -lcrypto
PROV = provided/_print_hex_buffer.c
HASH = sha256.c test/sha256-main.c
CREATE = ec_create.c test/ec_create-main.c
TOPUB = ec_to_pub.c ec_create.c test/ec_to_pub-main.c
FROMPUB = ec_from_pub.c ec_to_pub.c ec_create.c test/ec_from_pub-main.c
KEYSAVE = ec_save.c ec_create.c ec_to_pub.c test/ec_save-main.c
KEYLOAD = ec_load.c ec_save.c ec_create.c ec_to_pub.c test/ec_load-main.c
SIGN = ec_sign.c ec_create.c test/ec_sign-main.c
VERSIGN = ec_verify.c ec_sign.c ec_create.c test/ec_verify-main.c


hash:
	$(CC) $(CFLAGS) -o sha256-test $(HASH) $(PROV) $(LLIBS)

create:
	$(CC) $(CFLAGS) -o ec_create-test $(CREATE) $(PROV) $(LLIBS)

topub:
	$(CC) $(CFLAGS) -o ec_to_pub-test $(TOPUB) $(PROV) $(LLIBS)

frompub:
	$(CC) $(CFLAGS) -o ec_from_pub-test $(FROMPUB) $(PROV) $(LLIBS)

keysave:
	$(CC) $(CFLAGS) -o ec_save-test $(KEYSAVE) $(PROV) $(LLIBS)

keyload:
	$(CC) $(CFLAGS) -o ec_load-test $(KEYLOAD) $(PROV) $(LLIBS)

sign:
	$(CC) $(CFLAGS) -o ec_sign-test $(SIGN) $(PROV) $(LLIBS)

versign:
	$(CC) $(CFLAGS) -o ec_verify-test $(VERSIGN) $(PROV) $(LLIBS)

libhblk_crypto.a:
	$(CC) -c $(CFLAGS) *.c; ar -rc libhblk_crypto.a *.o
