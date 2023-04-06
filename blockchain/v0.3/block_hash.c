#include "blockchain.h"

/**
 * hash_id - Computes the hash of a transaction
 * @node:  Points to the transaction to be hashed
 * @index: Is the index of the transaction in the list
 * @arg:   Points to a buffer to store the resulting hash
 *
 * Return: Always 0
 */
static int hash_id(llist_node_t node, unsigned int index, void *arg)
{
	transaction_t *tx = node;
	uint8_t *hash_buf = arg;

	memcpy(&hash_buf[SHA256_DIGEST_LENGTH * index], tx->id, SHA256_DIGEST_LENGTH);
	return (0);
}

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
	int8_t *buf = NULL, *_buf = NULL;

	buf = _buf = calloc(1, (sizeof(block->info) + block->data.len) +
				   (llist_size(block->transactions) * SHA256_DIGEST_LENGTH));
	if (!buf)
		return (NULL);
	memcpy(buf, &block->info, sizeof(block->info) + block->data.len);
	buf += sizeof(block->info) + block->data.len;
	llist_for_each(block->transactions, hash_id, hash_buf);
	if (!sha256(_buf, sizeof(block->info) + block->data.len, hash_buf))
		return (NULL);
	free(_buf);
	return (hash_buf);
}
