#include "blockchain.h"
/* for ULONG_MAX */
#include <limits.h>

/* for memcpy */
#include <string.h>

/**
 * block_mine - Mines a Block in order to insert it in the Blockchain
 * @block: Points to the Block to be mined
 */
void block_mine(block_t *block)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	uint64_t i, nonce;

	nonce = block->info.nonce;

	for (i = 0; i < ULONG_MAX; i++)
	{
		block->info.nonce = i;

		block_hash(block, hash_buf);
		if (hash_matches_difficulty(hash_buf, block->info.difficulty))
		{
			memcpy(block->hash, hash_buf, SHA256_DIGEST_LENGTH);
			return;
		}
	}
	block->info.nonce = nonce;
}
