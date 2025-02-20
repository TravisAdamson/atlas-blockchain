#include "blockchain.h"

/**
 * hash_matches_difficulty - Verifies the given hash matches the given diff
 * @hash: hash to check
 * @difficulty: Difficulty of block to compare
 * Return: 1 if it matches, 0 if not
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
							uint32_t difficulty)
{
	uint8_t i = 0, count = 0, bit_count = 128;
	size_t j = 0;

	if (!hash)
		return (0);

	for (; i < SHA256_DIGEST_LENGTH; i++)
	{
		if (hash[i] == '\000')
			count += 8;
		else
			break;
	}
	for (; j < (sizeof(uint8_t) * 8); j++)
	{
		if ((hash[i] << j) & bit_count)
			break;
		count++;
	}
	return (count >= difficulty);
}
