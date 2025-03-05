#include "transaction.h"

/**
 * transaction_hash - Calculates the hash of a transaction
 * @transaction: transaction to hash
 * @hash_buf: buffer to hold the hash
 * Return: pointer to hash_buff or NULL
 */
uint8_t *transaction_hash(
	transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	int tot_ins = 0, tot_outs = 0;
	uint8_t *buff = NULL;

	if (!transaction || !hash_buf)
		return (NULL);
	tot_ins = llist_size(transaction->inputs);
	tot_outs = llist_size(transaction->outputs);
	buff = calloc(1, BUFF_SIZE);
	llist_for_each(transaction->inputs, hash_in, buff);
	llist_for_each(transaction->outputs, hash_out, &buff[32 * 3 * tot_ins]);
	SHA256(buff, BUFF_SIZE, hash_buf);
	free(buff), buff = NULL;
	return (hash_buf);
}

/**
 * hash_in - reads inputs into a buffer for hashing
 * @input: node in list
 * @i: index in list
 * @buff: Buffer to read into
 * Return: returns 0 on success, 1 on fail
 */
int hash_in(llist_node_t input, unsigned int i, void *buff)
{
	unsigned int pos = i * (32 * 3);

	if (memcpy(&((uint8_t *)buff)[pos], input, 96))
		return (0);
	return (1);
}

/**
 * hash_out - reads outputs into a buffer for hashing
 * @output: node out list
 * @i: index out list
 * @buff: Buffer to read into
 * Return: returns 0 on success, 1 on fail
 */
int hash_out(llist_node_t output, unsigned int i, void *buff)
{
	unsigned int pos = i * 32;

	if (memcpy(&((uint8_t *)buff)[pos], (uint8_t *)output + PTR_MOVE, 32))
		return (0);
	return (1);
}
