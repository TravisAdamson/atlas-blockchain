#include "transaction.h"

/**
 * update_unspent - Updates the unspent list as needed
 * @transactions: List of Valid Transactions
 * @block_hash: Hash of block that contains transaction
 * @all_unspent: list of unspent outputs
 * Return: Pointer to updated list
 */
llist_t *update_unspent(
	llist_t *transactions, uint8_t block_hash[SHA256_DIGEST_LENGTH],
	llist_t *all_unspent)
{
	ul_t context = {0};

	if (!transactions || !all_unspent || !block_hash)
		return (NULL);

	context.unspent = all_unspent;
	memcpy(context.hash, block_hash, SHA256_DIGEST_LENGTH);

	llist_for_each(transactions, (node_func_t)&sep_txi_txo, &context);

	return (all_unspent);
}

/**
 * sep_txi_txo - Seperates transactiion into input and output
 * @tx: transaction to seperate
 * @i: index of tx
 * @context: info needed
 * Return: 0 on success or 1 on fail
 */
int sep_txi_txo(transaction_t *tx, unsigned int i, ul_t *context)
{
	(void)i;

	memcpy(context->tx_id, tx->id, SHA256_DIGEST_LENGTH);
	llist_for_each(tx->inputs, (node_func_t)&check_out, context);
	llist_for_each(tx->outputs, (node_func_t)&update_out, context);
	return (0);
}

/**
 * check_out - checks for matching output in unspent
 * @in: input to check match for
 * @i: index of input
 * @context: info needed
 * Return: 0 on success or 1 on fail
 */
int check_out(ti_t *in, unsigned int i, ul_t *context)
{
	(void)i;

	llist_remove_node(context->unspent, (node_ident_t)&get_out, in, 1, NULL);
	return (0);
}

/**
 * get_out - checks the unspent nodes for a match
 * @unspent: unspent tx_out node
 * @in: input tx
 * Return: 1 on match or else 0
 */
int get_out(uto_t *unspent, tx_in_t *in)
{
	if (!memcmp(unspent->block_hash, in->block_hash, SHA256_DIGEST_LENGTH) &&
		!memcmp(unspent->tx_id, in->tx_id, SHA256_DIGEST_LENGTH) &&
		!memcmp(unspent->out.hash, in->tx_out_hash, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}

/**
 * update_out - Updates transactiion input/output
 * @out: node to add
 * @i: index of tx
 * @context: info needed
 * Return: 0 on success or 1 on fail
 */
int update_out(to_t *out, unsigned int i, ul_t *context)
{
	(void)i;
	uto_t *new_out;

	new_out = unspent_tx_out_create(context->hash, context->tx_id, out);
	llist_add_node(context->unspent, new_out, ADD_NODE_REAR);
	return (0);
}
