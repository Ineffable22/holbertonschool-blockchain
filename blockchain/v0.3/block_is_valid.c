#include "blockchain.h"

/**
 * block_is_valid - Verifies that a Block is valid
 * @block:      Points to the Block to check
 * @prev_block: Points to the previous Block in the Blockchain,
 *              or is NULL if block is the first Block of the chain
 * Return: 0 if valid, or 1 otherwise
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH] = {0};
	block_t const _genesis = GENESIS;

	if (!block || (!prev_block && block->info.index != 0))
		return (1);
	if (block->info.index == 0)
		return (memcmp(block, &_genesis, sizeof(block_t)));
	if (block->info.index != prev_block->info.index + 1)
		return (1);
	if (!block_hash(prev_block, hash_buf) ||
	    memcmp(hash_buf, prev_block->hash, SHA256_DIGEST_LENGTH))
		return (1);
	if (memcmp(prev_block->hash, block->info.prev_hash, SHA256_DIGEST_LENGTH))
		return (1);
	if (!block_hash(block, hash_buf) ||
	    memcmp(hash_buf, block->hash, SHA256_DIGEST_LENGTH))
		return (1);
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);
	return (0);
}
