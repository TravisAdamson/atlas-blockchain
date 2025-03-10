#include "transaction.h"

/**
 * transaction_is_valid - Validate given transaction
 * @transaction: Transaction to validate
 * @all_unspent: list of unspent transactions
 * Return: 1 if valid or 0 if not
 */
int transaction_is_valid(
	transaction_t const *transaction, llist_t *all_unspent)
{
	uint8_t v_hash[SHA256_DIGEST_LENGTH];
	tv_t context = {0};

	if (!transaction || !all_unspent)
		return (0);
	memcpy(context.tx_id, transaction->id, SHA256_DIGEST_LENGTH);
	context.unspent = all_unspent;
	transaction_hash(transaction, v_hash);
	if (memcmp(v_hash, transaction->id, SHA256_DIGEST_LENGTH))
		return (0);
	if (llist_for_each(transaction->inputs, (node_func_t)&valid_txi, &context))
		return (0);
	llist_for_each(transaction->outputs, (node_func_t)&get_txo_amount, &context);
	if (context.input != context.output)
		return (0);
	return (1);
}

/**
 * valid_txi - function for validate sig and unspent out
 * @in: input to check
 * @i: iterator needed for llist functions
 * @context: struct holding tx and unspent list
 * Return: 0 on match or 1 on fail
 */
int valid_txi(tx_in_t *in, uint32_t i, tv_t *context)
{
	(void)i;
	uto_t *match_found = NULL;
	EC_KEY *key = NULL;

	match_found = llist_find_node(context->unspent,
		(node_ident_t)&verify_hash, in);
	if (!match_found)
		return (1);
	context->input += match_found->out.amount;
	key = ec_from_pub(match_found->out.pub);
	if (!key)
		return (1);
	if (!ec_verify(key, context->tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (EC_KEY_free(key), 1);
	EC_KEY_free(key);
	return (0);
}

/**
 * verify_hash - checks the unspent nodes for a matching hash
 * @unspent: unspent tx_out nodes
 * @in: input tx
 * Return: 1 on match or 0 on fail
 */
int verify_hash(uto_t *unspent, tx_in_t *in)
{
	if (!memcmp(unspent->block_hash, in->block_hash, SHA256_DIGEST_LENGTH) &&
		!memcmp(unspent->tx_id, in->tx_id, SHA256_DIGEST_LENGTH) &&
		!memcmp(unspent->out.hash, in->tx_out_hash, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}

/**
 * get_txo_amount - finds output amounts
 * @out: input to check
 * @i: iterator needed for llist functions
 * @context: struct holding tx and unspent list
 * Return: 0 on match or 1 on fail
 */
int get_txo_amount(tx_out_t *out, unsigned int i, tv_t *context)
{
	(void)i;
	context->output += out->amount;
	return (0);
}
