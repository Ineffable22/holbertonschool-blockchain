#include "blockchain.h"

/**
 * block_create - Creates a Block structure and initializes it.
 * @prev:     Is a pointer to the previous Block in the Blockchain.
 * @data:     Points to a memory area to duplicate in the Block’s data.
 * @data_len: Stores the number of bytes to duplicate in data.
 *            If data_len is bigger than BLOCKCHAIN_DATA_MAX, then only
 *            BLOCKCHAIN_DATA_MAX bytes must be duplicated.
 *
 * Return: A pointer to the allocated Block
 */
block_t *block_create(block_t const *prev, int8_t const *data,
		      uint32_t data_len)
{
	block_t *block = calloc(1, sizeof(block_t));

	if (!block || !prev)
		return (NULL);
	(block->info).index = (prev->info).index + 1;
	(block->info).timestamp = time(NULL);
	memcpy(&((block->info).prev_hash), (prev->hash), SHA256_DIGEST_LENGTH);
	memcpy(&((block->data).buffer), data, BLOCKCHAIN_DATA_MAX);
	(block->data).len = MIN(data_len, BLOCKCHAIN_DATA_MAX);
	block->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!block->transactions)
		return (free(block), NULL);
	return (block);
}
