#include "blockchain.h"

/**
 * block_hash - Computes the hash of a Block
 * @block:    Points to the Block to be hashed
 * @hash_buf: The resulting hash must be stored in hash_buf
 *
 * Return: A pointer to hash_buf
 */
uint8_t *block_hash(block_t const *block,
		    uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	if (!block || !hash_buf)
		return (NULL);
	if (!sha256((const int8_t *)&(block->info),
		    sizeof(block->info) + block->data.len, hash_buf))
		return (NULL);
	return (hash_buf);
}
