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
	size_t total_tx = 0, buffer_len = 0, blk_size = 0;
	uint8_t *buffer;

	if (!block || !hash_buf)
		return (NULL);
	if (block->transactions)
		total_tx = llist_size(block->transactions);
	buffer_len = sizeof(block->info) + block->data.len +
				 (total_tx * SHA256_DIGEST_LENGTH);
	blk_size = sizeof(block->info) + block->data.len;
	buffer = calloc(1, buffer_len);
	memcpy(buffer, block, blk_size);
	llist_for_each(block->transactions, dup_tx_id, buffer + blk_size);

	SHA256(buffer, buffer_len, hash_buf);
	return (hash_buf);
}

/**
 * dup_tx_id - loads tx_id into buffer
 * @tx: Transaction to store to buffer
 * @i: index of transaction
 * @buffer: Buffer to copy into
 * Return: 1 on Fail or 0 on success
 */
int dup_tx_id(llist_node_t tx, unsigned int i, void *buffer)
{
	memcpy(
		(uint8_t *)buffer + i * SHA256_DIGEST_LENGTH,
		((transaction_t *)tx)->id, SHA256_DIGEST_LENGTH);
	return (0);
}
