#include "transaction.h"

/**
 * unspent_tx_out_create - creates an unspent transaction struct
 * @block_hash: hash of block where transaction is at
 * @tx_id: Transaction ID
 * @out: Transaction output
 * Return: NULL or pointer to new unspent transaction
 */
uto_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH], uint8_t tx_id[SHA256_DIGEST_LENGTH],
	to_t const *out)
{
	uto_t *new_utxo;

	if (!block_hash || !tx_id || !out)
		return (NULL);
	new_utxo = calloc(1, sizeof(uto_t));
	if (!new_utxo)
		return (NULL);
	memcpy(new_utxo->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(new_utxo->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	new_utxo->out = *out;
	return (new_utxo);
}
