#include "hblk_crypto.h"

/**
 * sha256 - Computes the hash of a given sequence of bytes
 * @s: sequence of bytes to hash
 * @len: num of bytes to hash in s
 * @digest: array to store the resulting hash
 * Return: Pointer to digest hash, or NULL
 */
uint8_t *sha256(int8_t const *s, size_t len,
                uint8_t digest[SHA256_DIGEST_LENGTH])
{
    SHA256_CTX results_data;

    if (!SHA256_Init(&results_data))
        return (NULL);
    if (!SHA256_Update(&results_data, s, len))
        return (NULL);
    if (!SHA256_Final(digest, &results_data))
        return (NULL);
    if (!digest)
        return (NULL);
    else
        return (digest);
}
