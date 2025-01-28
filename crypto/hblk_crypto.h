#ifndef _HBLK_CRYPTO_H_
#define _HBLK_CRYPTO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <openssl/ssl.h>
#include <openssl/sha.h>
#include <openssl/obj_mac.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/ecdsa.h>
#include <openssl/bio.h>
#include <openssl/ecdh.h>

uint8_t *sha256(int8_t const *s, size_t len,
				uint8_t digest[SHA256_DIGEST_LENGTH]);

#endif
