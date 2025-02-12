#include "blockchain.h"

/**
 * block_hash - finds the hash for a block using sha256
 * @block: block to hash
 * @hash_buf: buffer to store computed hash
 * Return: hash buffer or NULL
 */
uint8_t *block_hash(block_t const *block,
					uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	if (!block || !hash_buf)
		return (NULL);
	SHA256((unsigned char *)block, sizeof(block->info) + block->data.len,
			hash_buf);
	return (hash_buf);
}
