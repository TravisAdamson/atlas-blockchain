#include "hblk_crypto.h"

/**
 *  ec_from_pub - Creates an EC_key from the given public key
 * @pub: Public key given
 * Return: pointer to EC_key or NULL
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key = NULL;
	EC_POINT *point = NULL;
	const EC_GROUP *group = NULL;

	if (!pub)
		return (NULL);

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	group = EC_KEY_get0_group(key);
	point = EC_POINT_new(group);
	EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, NULL);
	if (!group || !point)
		return (NULL);

	if (!EC_KEY_set_public_key(key, point))
		return (NULL);

	EC_POINT_free(point);
	
	return (key);
}
