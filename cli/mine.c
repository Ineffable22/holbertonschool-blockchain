#include "cli.h"

/**
 * check_args - Check if the arguments are valid
 * @arg:      Arguments
 * @session:  Session struct
 *
 * Return: 0 if valid, 1 if not
 */
static int check_args(char **arg, session_t *session)
{
	if (arg[1] != NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: too many arguments\n"
				     "Usage: mine\n";
		return (0);
	}
	if (session->blockchain == NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: blockchain not loaded";
		return (0);
	}
	if (session->wallet == NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: wallet not loaded";
		return (0);
	}
	if (llist_size(session->tx_pool) == 0)
	{
		session->state.code = 0;
		session->state.msg = "Error: transaction pool empty";
		return (0);
	}
	return (1);
}

/**
 * _add_block - add block
 * @block:   block
 * @session: session
 *
 * Return: 0 if valid, 1 if not
 */
int _add_block(block_t *block, session_t *session)
{
	while (!llist_remove_node(session->tx_pool,
				  (node_ident_t)invalid_tx, session->blockchain->unspent,
				  1, (node_dtor_t)transaction_destroy))
		fprintf(stderr, "Error: invalid tx destroyed.\n");
	if (llist_for_each(session->tx_pool, add_tx, block))
		return (1);
	return (0);
}

/**
 * mine - Mines a block
 * @arg:      Arguments
 * @session:  Session struct
 *
 * Return: Nothing
 */
void mine(char **arg, session_t *session)
{
	block_t *block = NULL;
	transaction_t *coin_tx = NULL;
	tx_out_t *tx_out;
	unspent_tx_out_t *utx;

	if (!check_args(arg, session))
		return;
	block = new_block(session, block, session->wallet->key, &coin_tx);
	if (!block || !coin_tx)
		return;

	session->blockchain->unspent = update_unspent(session->tx_pool,
						      block->hash, session->blockchain->unspent);
	if (!session->blockchain->unspent)
	{
		session->state.code = 0;
		session->state.msg = "Error: failed to create miner";
		return;
	}
	llist_destroy(session->tx_pool, 1, (node_dtor_t)transaction_destroy);
	session->tx_pool = llist_create(MT_SUPPORT_FALSE);
	tx_out = llist_get_head(coin_tx->outputs);
	if (!tx_out)
	{
		session->state.code = 1;
		session->state.msg = "Error: failed to get coinbase output";
		block_destroy(block);
		return;
	}
	utx = unspent_tx_out_create(block->hash, coin_tx->id, tx_out);
	if (!utx || llist_add_node(session->blockchain->unspent,
				   utx, ADD_NODE_REAR))
	{
		SC(session, 1, "Error: failed to add coinbase output");
		block_destroy(block);
		return;
	}
	SC(session, 0, "Block mined successfully");
}

/**
 * new_block - create new block
 * @session:  Session
 * @block:    Block
 * @miner:    Miner
 * @coin_tx:  Coin tx
 *
 * Return: Nothing
 */
block_t *new_block(session_t *session, block_t *block, EC_KEY *miner,
		   transaction_t **coin_tx)
{
	char *str = "Holberton School";
	block_t *prev;

	prev = llist_get_tail(session->blockchain->chain);
	block = block_create(prev, (int8_t *)str, (uint32_t)strlen(str));

	// if (_add_block(block, session))
	// {
	// 	session->state.code = 0;
	// 	session->state.msg = "Error: Empty pool for session data";
	// 	return (NULL);
	// }
	block->info.difficulty = blockchain_difficulty(session->blockchain);
	*coin_tx = coinbase_create(miner, block->info.index);
	if (!*(coin_tx) || !coinbase_is_valid(*(coin_tx), block->info.index))
	{
		session->state.code = 1;
		session->state.msg = "Error: failed to create coinbase";
		transaction_destroy(*(coin_tx));
		block_destroy(block);
		return (NULL);
	}
	llist_add_node((block)->transactions, *(coin_tx), ADD_NODE_FRONT);
	block_mine(block);
	printf("MINED BLOCK:\n");
	_block_print(block, 0, "\t\t");
	if (block_is_valid(block, prev, session->blockchain->unspent))
	{
		session->state.code = 1;
		session->state.msg = "Error: failed to mine block";
		block_destroy(block);
		return (NULL);
	}
	llist_add_node(session->blockchain->chain,
		       block, ADD_NODE_REAR);
	printf("Block mined: [%u] ", block->info.difficulty);
	_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
	printf("\n");
	return (block);
}
