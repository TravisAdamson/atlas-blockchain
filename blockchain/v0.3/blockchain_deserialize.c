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
	uint32_t numblocks, total_txu, total_tx, i = 0;
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
	fread(&total_txu, 4, 1, f_ptr);
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);

	for (; i < numblocks; i++)
	{
		block = calloc(1, sizeof(block_t));
		fread(&block->info, 1, sizeof(block_info_t), f_ptr);
		fread(&(block->data.len), sizeof(uint8_t), 4, f_ptr);
		fread(&block->data.buffer, block->data.len, sizeof(uint8_t), f_ptr);
		fread(&block->hash, sizeof(uint8_t), SHA256_DIGEST_LENGTH, f_ptr);
		fread(&total_tx, 4, 1, f_ptr);
		if ((int)total_tx != -1)
		{
			block->transactions = llist_create(MT_SUPPORT_FALSE);
			read_tx(f_ptr, total_tx, block->transactions);
		}
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}
	read_txu(f_ptr, blockchain, total_txu);
	fclose(f_ptr);
	return (blockchain);
}

/**
 * read_tx - Reads the transaction info from file
 * @f_ptr: file pointer to file to read from
 * @total_tx: number of transactions
 * @tx_list: list of transactions
 * Return: 1 on success
 */
int read_tx(FILE *f_ptr, uint32_t total_tx, llist_t *tx_list)
{
	uint32_t i = 0, total_txi, total_txo;
	transaction_t *tx;

	if (!total_tx)
		return (1);

	for (; i < total_tx; i++)
	{
		tx = calloc(1, sizeof(transaction_t));
		fread(tx->id, sizeof(uint8_t), SHA256_DIGEST_LENGTH, f_ptr);
		fread(&total_txi, 4, 1, f_ptr);
		fread(&total_txo, 4, 1, f_ptr);
		tx->inputs = llist_create(MT_SUPPORT_FALSE);
		tx->outputs = llist_create(MT_SUPPORT_FALSE);
		read_txi(f_ptr, total_txi, tx->inputs);
		read_txo(f_ptr, total_txo, tx->outputs);
		llist_add_node(tx_list, tx, ADD_NODE_REAR);
	}
	return (1);
}

/**
 * read_txi - Reads tx inputs from file
 * @f_ptr: file pointer
 * @total_txi: number of inputs
 * @tx_in: list of inputs to add to
 * Return: 1
 */
int read_txi(FILE *f_ptr, uint32_t total_txi, llist_t *tx_in)
{
	uint32_t i = 0;
	ti_t *in;

	if (!total_txi)
		return (1);

	for (; i < total_txi; i++)
	{
		in = calloc(1, sizeof(ti_t));
		fread(in->block_hash, 32, 1, f_ptr);
		fread(in->tx_id, 32, 1, f_ptr);
		fread(in->tx_out_hash, 32, 1, f_ptr);
		fread(in->sig.sig, 72, 1, f_ptr);
		fread(&in->sig.len, 1, 1, f_ptr);
		llist_add_node(tx_in, in, ADD_NODE_REAR);
	}
	return (1);
}

/**
 * read_txo - Reads tx outputs from file
 * @f_ptr: file pointer
 * @total_txo: number of outputs
 * @tx_out: list of outputs to add to
 * Return: 1
 */
int read_txo(FILE *f_ptr, uint32_t total_txo, llist_t *tx_out)
{
	uint32_t i = 0;
	to_t *out;

	if (!total_txo)
		return (1);

	for (; i < total_txo; i++)
	{
		out = calloc(1, sizeof(ti_t));
		fread(&out->amount, 4, 1, f_ptr);
		fread(out->pub, 65, 1, f_ptr);
		fread(out->hash, 32, 1, f_ptr);
		llist_add_node(tx_out, out, ADD_NODE_REAR);
	}
	return (1);
}

/**
 * read_txu - Reads unspent outputs from file
 * @f_ptr: file pointer
 * @blockchain: blockchain to add list to
 * @total_txu: number of unspent tx
 * Return: 1
 */
int read_txu(FILE *f_ptr, blockchain_t *blockchain, uint32_t total_txu)
{
	uto_t *unspent;
	uint32_t i = 0;

	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!total_txu)
		return (1);

	for (; i < total_txu; i++)
	{
		unspent = calloc(1, sizeof(uto_t));
		fread(unspent->block_hash, 32, 1, f_ptr);
		fread(unspent->tx_id, 32, 1, f_ptr);
		fread(&unspent->out.amount, 4, 1, f_ptr);
		fread(unspent->out.pub, 65, 1, f_ptr);
		fread(unspent->out.hash, 32, 1, f_ptr);
		llist_add_node(blockchain->unspent, unspent, ADD_NODE_REAR);
	}

	return (1);
}
