#include "hblk_crypto.h"

/**
 * ec_save - saves the given EC key pair in given location
 * @key: key to save
 * @folder: folder to save the key in
 * Return: 1 on success or 0 on fail
 */
int ec_save(EC_KEY *key, char const *folder)
{
	struct stat info;
	char pub_path[1024] = {0}, priv_path[1024] = {0};
	FILE *file_ptr;

	if (!key || !folder)
		return (0);
	if (stat(folder, &info) || !S_ISDIR(info.st_mode))
		mkdir(folder, 0755);
	memcpy(pub_path, folder, strlen(folder));
	memcpy(&pub_path[strlen(folder)], "/key_pub.pem", 12);
	memcpy(priv_path, folder, strlen(folder));
	memcpy(&priv_path[strlen(folder)], "/key.pem", 8);
	file_ptr = fopen(pub_path, "w+");
	PEM_write_EC_PUBKEY(file_ptr, key), fclose(file_ptr);
	file_ptr = fopen(priv_path, "w+");
	PEM_write_ECPrivateKey(file_ptr, key, NULL, NULL, 0, NULL, NULL), fclose(file_ptr);
	return (1);
}
