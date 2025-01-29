#include "hblk_crypto.h"

/**
 * ec_verify - Verifies the provided signature
 * @key: Key to verify signature
 * @msg: message that was signed
 * @msglen: length of signed message
 * @sig: struct holding the given signature
 * Return: 1 if valid, 0 if not
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen,
			  sig_t const *sig)
{
	if (!key || !msg || !sig || !sig->len)
		return (0);

	return (ECDSA_verify(0, msg, msglen, sig->sig, sig->len, (EC_KEY *)key));
}
