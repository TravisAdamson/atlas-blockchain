#include "blockchain.h"

/**
 * blockchain_serialize - saves a blockchain to file
 * @blockchain: chain to save
 * @path: file path to save to
 * Return: 1 on succeess, 0 on fail
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *f_ptr = NULL;
	int blocknums = 0;
	char header[12] = {FHEADER};

	if (!blockchain || !path)
		return (0);
	blocknums = llist_size(blockchain->chain);
	memcpy(&header[7], END, 1);
	memcpy(&header[8], &blocknums, 4);
	f_ptr = fopen(path, "w");
	fwrite(header, 1, 12, f_ptr);
	llist_for_each(blockchain->chain, write_each, f_ptr);
	fclose(f_ptr);
	return (1);
}

/**
 * write_each - function to write blocks to file
 * @list: node to perform function on
 * @index: unused
 * @arg: filestream pointer
 * Return: 0
 */
int write_each(llist_node_t list, unsigned int index, void *arg)
{
	block_t *temp = NULL;
	char block_buff[1116];
	uint32_t len = 0;

	(void)index;

	temp = (block_t *)list;
	if (!strcmp(END, "\x02"))
		SWAPENDIAN(list);

	len = temp->data.len;

	memcpy(&block_buff[0], temp, sizeof(block_info_t));
	memcpy(&block_buff[56], &temp->data.len, 4);
	memcpy(&block_buff[60], temp->data.buffer, len);
	memcpy(&block_buff[60 + len], temp->hash, 32);
	fwrite(block_buff, 1, 92 + len, arg);
	return (0);
}
