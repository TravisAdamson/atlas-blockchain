#include "blockchain.h"

/**
 * block_destroy - Destroys the given block
 * @block: block to destroy
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;
	free(block);
}
