#include "blockchain.h"

/**
 * check_tx - Checks if a transaction is valid
 * @node:  Points to the transaction to check
 * @index: Is the index of the transaction in the list
 * @arg:   Points to a structure containing the list of all unspent
 *         transaction outputs, and a flag indicating whether the
 *         transaction is a coinbase transaction
 *
 * Return: 0 if valid, or 1 otherwise
 */
static int check_tx(llist_node_t node, unsigned int index, void *arg)
{
	transaction_t *tx = node;
	tx_info_t *info = arg;

	if (!index)
	{
		if (!coinbase_is_valid(tx, info->bool))
			return (1);
	}
	else if (!transaction_is_valid(tx, info->all_unspent))
		return (1);
	return (0);
}

/**
 * block_is_valid - Verifies that a Block is valid
 * @block:       Points to the Block to check
 * @prev_block:  Points to the previous Block in the Blockchain,
 *               or is NULL if block is the first Block of the chain
 * @all_unspent: Points to the list of all unspent transaction outputs
 * Return: 0 if valid, or 1 otherwise
 */
int block_is_valid(block_t const *block, block_t const *prev_block, llist_t *all_unspent)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH] = {0};
	block_t const _genesis = GENESIS;
	tx_info_t info = {0};

	if (!block || (!prev_block && block->info.index != 0))
		return (1);
	if (block->info.index == 0)
		return (memcmp(block, &_genesis, sizeof(block_t)));
	if (block->info.index != prev_block->info.index + 1)
		return (1);
	if (!block_hash(prev_block, hash_buf))
		return (1);
	if (memcmp(hash_buf, prev_block->hash, SHA256_DIGEST_LENGTH))
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
	if (llist_size(block->transactions) < 1)
		return (1);
	info.all_unspent = all_unspent;
	info.bool = block->info.index;
	if (llist_for_each(block->transactions, check_tx, &info))
		return (1);
	return (0);
}
