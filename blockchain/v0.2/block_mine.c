#include "blockchain.h"

/**
 * block_mine - Mines a Block in order to insert it in the Blockchain
 * @block: Points to the Block to be mined
 */
void block_mine(block_t *block)
{
	block->info.nonce = 0;
	do {
		block->info.nonce += 1;
		block_hash(block, block->hash);
	} while (!hash_matches_difficulty(block->hash, block->info.difficulty));
}
