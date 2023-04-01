#include "blockchain.h"

/**
 * blockchain_destroy - Deletes an existing Blockchain, along with all
 * the Blocks it contains
 *
 * @blockchain: Points to the Blockchain structure to delete
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;
	llist_destroy(blockchain->chain, 1, (void (*)(llist_node_t))block_destroy);
	free(blockchain);
}
