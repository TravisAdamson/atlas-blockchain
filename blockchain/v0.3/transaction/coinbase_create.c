#include "transaction.h"

/**
 * coinbase_create - Creates a coinbase transaction
 * @receiver: key of the receiver
 * @block_index: Index of block coinbase belongs to
 * Return: pointer to new transaction or NULL
 */
transaction_t *coinbase_create(
	EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *new_cbtx;
	to_t *txo;
	ti_t *txi;
	uint8_t pub[EC_PUB_LEN];

	if (!receiver)
		return (NULL);
	new_cbtx = calloc(1, sizeof(transaction_t));
	if (!new_cbtx)
		return (NULL);
	new_cbtx->inputs = llist_create(MT_SUPPORT_FALSE);
	new_cbtx->outputs = llist_create(MT_SUPPORT_FALSE);
	ec_to_pub(receiver, pub);
	txo = tx_out_create(COINBASE_AMOUNT, pub);
	txi = calloc(1, sizeof(ti_t));
	if (!txi)
	{
		free(new_cbtx);
		free(txo);
		return (NULL);
	}
	memcpy(txi->tx_out_hash, &block_index, sizeof(uint32_t));
	llist_add_node(new_cbtx->inputs, txi, ADD_NODE_REAR);
	llist_add_node(new_cbtx->outputs, txo, ADD_NODE_REAR);
	transaction_hash(new, new_cbtx->id);
	return (new);
}
