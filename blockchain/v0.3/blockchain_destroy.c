#include "blockchain.h"

/**
 * blockchain_destroy - destroys an entire blockchain
 * @blockchain: chain to destroy
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;
	llist_destroy(blockchain->unspent, 1, NULL);
	llist_destroy(blockchain->chain, 1, (node_dtor_t)&block_destroy);
	free(blockchain);
}
