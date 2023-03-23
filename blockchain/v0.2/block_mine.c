#include "blockchain.h"

/**
 * block_mine - Mines a Block in order to insert it in the Blockchain
 * @block: Points to the Block to be mined
 */
void block_mine(block_t *block)
{
	uint64_t nounce = 0;

	while (!hash_matches_difficulty(block->hash, block->info.difficulty))
	{
		block->info.nonce = nounce;
		block_hash(block, block->hash);
		nounce++;
	}
}
