#include "cli.h"

/**
 * add_tx - add tx
 * @node:   Node
 * @idx:    Index
 * @_block: Block
 *
 * Return: 0 if valid, 1 if not
 */
int add_tx(llist_node_t node, unsigned int idx, void *_block)
{
	transaction_t *tx = node;
	block_t *block = _block;

	(void)idx;
	if (llist_add_node(block->transactions, tx, ADD_NODE_REAR))
	{
		dprintf(2, "add_tx: error llist_add_node!\n");
	}
	return (0);
}

/**
 * invalid_tx - invalid tx
 * @node:    node
 * @unspent: unspent
 *
 * Return: 0 if valid, 1 if not
 */
int invalid_tx(llist_node_t node, void *unspent)
{
	transaction_t *tx = node;

	printf("delete_identifier:\n");
	_print_hex_buffer(tx->id, SHA256_DIGEST_LENGTH);
	printf("\n");
	return (!transaction_is_valid(tx, unspent));
}
