#include "hblk_crypto.h"

/**
 * ec_load - Loads a key from the provided file
 * @folder: Folder to load from
 * Return: Pointer to EC_KEY or NULL
 */
EC_KEY *ec_load(char const *folder)
{
	struct stat data;
	char pub_path[1024] = {0}, priv_path[1024] = {0};
	FILE *file_pointer;
	EC_KEY *key = NULL;

	if (!folder)
		return (NULL);
	if (stat(folder, &data) || (!stat(folder, &data) && !S_ISDIR(data.st_mode)))
		return (NULL);

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	memcpy(pub_path, folder, strlen(folder));
	memcpy(&pub_path[strlen(folder)], "/key_pub.pem", 12);
	memcpy(priv_path, folder, strlen(folder));
	memcpy(&priv_path[strlen(folder)], "/key.pem", 8);

	file_pointer = fopen(pub_path, "r");
	if (!file_pointer)
		return (NULL);
	PEM_read_EC_PUBKEY(file_pointer, &key, NULL, NULL), fclose(file_pointer);

	file_pointer = fopen(priv_path, "r");
	if (!file_pointer)
		return (NULL);
	PEM_read_ECPrivateKey(file_pointer, &key, NULL, NULL), fclose(file_pointer);

	return (key);
}
