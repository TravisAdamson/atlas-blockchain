#include "blockchain.h"

/**
 * blockchain_deserialize - Loads the blockchain from a given file
 * @path: file to read from
 * Return: Pointer to chain or NULL
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *f_ptr = NULL;
	char header_buff[7] = {0};
	uint8_t end;
	uint32_t numblocks, i = 0;
	blockchain_t *blockchain = calloc(1, sizeof(blockchain_t));
	block_t *block = NULL;

	if (!path)
		return (NULL);
	f_ptr = fopen(path, "r");
	if (!f_ptr)
		return (NULL);
	fread(header_buff, 1, 7, f_ptr);
	if (memcmp(header_buff, FHEADER, 7))
		return (NULL);
	fread(&end, 1, 1, f_ptr);
	fread(&numblocks, 4, 1, f_ptr);
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);

	for (; i < numblocks; i++)
	{
		block = calloc(1, sizeof(block_t));
		fread(&block->info, 1, sizeof(block_info_t), f_ptr);
		fread(&(block->data.len), sizeof(uint8_t), 4, f_ptr);
		fread(&block->data.buffer, sizeof(uint8_t), block->data.len, f_ptr);
		fread(&block->hash, sizeof(uint8_t), SHA256_DIGEST_LENGTH, f_ptr);
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}
	fclose(f_ptr);
	return (blockchain);
}
