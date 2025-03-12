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
	int blocknums = 0, total_unspent = 0;
	char header[16] = {FHEADER};

	if (!blockchain || !path)
		return (0);
	blocknums = llist_size(blockchain->chain);
	total_unspent = llist_size(blockchain->unspent);
	memcpy(&header[7], END, 1);
	memcpy(&header[8], &blocknums, 4);
	memcpy(&header[12], &total_unspent, 4);
	f_ptr = fopen(path, "w");
	fwrite(header, 1, 16, f_ptr);
	llist_for_each(blockchain->chain, (node_func_t)&write_block, f_ptr);
	llist_for_each(blockchain->unspent, (node_func_t)&write_txu, f_ptr);
	fclose(f_ptr);
	return (1);
}

/**
 * write_block - function to write blocks to file
 * @block: block to perform function on
 * @i: unused
 * @f_ptr: filestream pointer
 * Return: 0
 */
int write_block(block_t *block, unsigned int i, FILE *f_ptr)
{
	char block_buff[1120];
	uint32_t len = 0;
	int tx_size = 0;

	(void)i;

	tx_size = llist_size(block->transactions);
	if (!strcmp(END, "\x02"))
		SWAPENDIAN(block);

	len = block->data.len;

	memcpy(&block_buff[0], block, sizeof(block_info_t));
	memcpy(&block_buff[56], &temp->data.len, 4);
	memcpy(&block_buff[60], temp->data.buffer, len);
	memcpy(&block_buff[60 + len], temp->hash, 32);
	memcpy(&block_buff[92 + len], &tx_size, 4);
	fwrite(block_buff, 1, 92 + len, f_ptr);
	if (tx_size > 0)
		llist_for_each(block->transactions, (node_func_t)*write_tx, f_ptr);
	return (0);
}

/**
 * write_tx - writes tx node to file
 * @tx: transaction to write
 * @i: unused
 * @f_ptr: file pointer
 * Return: 0
 */
int write_tx(transaction_t *tx, unsigned int i, FILE *f_ptr)
{
	(void)i;
	char tx_buff[40];
	int txi = 0, txo = 0;

	txi = llist_size(tx->inputs);
	txo = llist_size(tx->outputs);

	memcpy(&tx_buff[0], tx->id, 32);
	memcpy(&tx_buff[32], &txi, 4);
	memcpy(&tx_buff[36], &txo, 4);
	fwrite(&tx_buff, 1, 40, f_ptr);
	llist_for_each(tx->inputs, (node_func_t)&write_txi, f_ptr);
	llist_for_each(tx->outputs, (node_func_t)&write_txo, f_ptr);
	return (0);
}

/**
 * write_txi - writes inputs to file
 * @in: input to write
 * @i: unused
 * @f_ptr: file pointer
 * Return: 0
 */
int write_txi(ti_t *in, unsigned int i, FILE *f_ptr)
{
	(void)i;
	char txi_buff[169];

	memcpy(&txi_buff[0], in->block_hash, 32);
	memcpy(&txi_buff[32], in->tx_id, 32);
	memcpy(&txi_buff[64], in->tx_out_hash, 32);
	memcpy(&txi_buff[96], in->sig.sig, 72);
	memcpy(&txi_buff[168], &in->sig.len, 1);
	fwrite(&txi_buff, 1, 169, f_ptr);
	return (0);
}

/**
 * write_txo - writes outputs to file
 * @out: output to write
 * @i: unused
 * @f_ptr: File pointer
 * Return: 0
 */
int write_txo(to_t *out, unsigned int i, FILE *f_ptr)
{
	(void)i;
	char txo_buff[101];

	memcpy(&txo_buff[0], &out->amount, 4);
	memcpy(&txo_buff[4], out->pub, 65);
	memcpy(&txo_buff[69], out->hash, 32);
	fwrite(&txo_buff, 1, 101, f_ptr);
	return (0);
}

/**
 * write_txu - writes unspent outputs to file
 * @unspent: unspent to write
 * @index: unsused
 * @fptr: file pointer
 * Return: 0
 */
int write_txu(uto_t *unspent, unsigned int i, FILE *f_ptr)
{
	(void)i;
	char txu_buff[165];

	memcpy(&txu_buff[0], unspent->block_hash, 32);
	memcpy(&txu_buff[32], unspent->tx_id, 32);
	memcpy(&txu_buff[64], &unspent->out.amount, 4);
	memcpy(&txu_buff[68], unspent->out.pub, 65);
	memcpy(&txu_buff[133], unspent->out.hash, 32);
	fwrite(&txu_buff, 1, 165, f_ptr);
	return (0);
}
