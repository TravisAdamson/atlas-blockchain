#include "hblk_crypto.h"

/**
 * ec_to_pub - Retrieves the public key from the given EC_KEY pointer
 * @key: pointer to get key from
 * @pub: array to store public key
 * Return: Pub or NULL on fail
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_POINT *point = NULL;
	const EC_GROUP *group = NULL;
	point_conversion_form_t form;

	if (!key)
		return (NULL);

	point = EC_KEY_get0_public_key(key);
	group = EC_KEY_get0_group(key);
	form = EC_KEY_get_conv_form(key);
	if (!point || !group || !form)
		return (NULL);

	EC_POINT_point2oct(group, point, form, pub, EC_PUB_LEN, NULL);

	if (pub)
		return (pub);
	else
		return (NULL);
}
