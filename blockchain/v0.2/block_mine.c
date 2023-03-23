#include "blockchain.h"

/**
 * block_mine - Mines a Block in order to insert it in the Blockchain
 * @block: Points to the Block to be mined
 */
void block_mine(block_t *block)
{
	uint64_t nonce = 0;

	block->info.nonce = nonce;
	block_hash(block, block->hash);
	while (!hash_matches_difficulty(block->hash, block->info.difficulty))
	{
		nonce++;
		block->info.nonce = nonce;
		block_hash(block, block->hash);
	}
}
