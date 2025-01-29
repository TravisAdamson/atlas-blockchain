#include "hblk_crypto.h"

/**
 * ec_create - Creates a new keyed pair using secp256k1 elliptic curve
 * Return: Pointer to EC Key struct or NULL
 */
EC_KEY *ec_create(void)
{
	EC_KEY *key = NULL;

	key = EC_KEY_new_by_curve_name(NID_secp256k1);
	if (!key)
		return (NULL);

	return (EC_KEY_generate_key(key) ? key : NULL);
}
