#include "blockchain.h"

/**
 * block_destroy - Deletes an existing Block
 *
 * @block: Points to the Block to delete
 */
void block_destroy(block_t *block)
{
	free(block);
}
