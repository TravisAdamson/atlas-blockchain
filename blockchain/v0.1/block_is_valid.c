#include "blockchain.h"

/**
 * block_is_valid - Validates the given block
 * @block: block to validate
 * @prev_block: block before block to validate
 * Return: 0 on Success, 1 on fail
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t prev_hash[SHA256_DIGEST_LENGTH] = {0};
	uint8_t current_hash[SHA256_DIGEST_LENGTH] = {0};

	if (!block)
		return (1);

	if (!prev_block && block->info.index != 0)
		return (1);

	if (block->info.index == 0)
	{
		if (genesis_blk(block))
			return (1);
		else
			return (0);
	}

	if (block->info.index != prev_block->info.index + 1)
		return (1);

	block_hash(prev_block, prev_hash);
	if (memcmp(prev_hash, prev_block->hash, 32))
		return (1);

	if (memcmp(prev_block->hash, block->info.prev_hash, 32))
		return (1);

	block_hash(block, current_hash);
	if (memcmp(current_hash, block->hash, 32))
		return (1);

	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (0);

	return (0);
}

/**
 * genesis_blk - Checks if the block is a genesis block
 * @block: block to check
 * Return: 1 on success, 0 on fail
 */
int genesis_blk(block_t const *block)
{
	block_t genesis = {
		{0, 0, 1537578000, 0, {0}},
		{"Holberton School", 16},
		ATLAS_HASH};

	return (memcmp(&genesis, block, 1116));
}
