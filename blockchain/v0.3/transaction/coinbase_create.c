#include "transaction.h"

/**
 * coinbase_create - Create a coinbase transaction
 * @receiver:    Contains the public key of the miner,
 *               who will receive the coinbase coins
 * @block_index: Is the index of the Block the coinbase transaction will belong
 *
 * Return: A pointer to the created transaction upon success,
 * or NULL upon failure
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *tx = NULL;
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	uint8_t pub[EC_PUB_LEN];

	/* Check parameters */
	if (!receiver || block_index < 1 || !ec_to_pub(receiver, pub))
		return (NULL);
	tx = calloc(1, sizeof(transaction_t));
	if (!tx)
		return (NULL);
	/* Create transaction inputs and outputs lists */
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!tx->inputs || !tx->outputs)
		return (NULL);
	/* Create coinbase transaction input */
	tx_in = calloc(1, sizeof(tx_in_t));
	if (!tx_in)
		return (NULL);
	/* Set block hash to block index */
	memcpy(tx_in->tx_out_hash, &block_index, 4);

	/* Add coinbase transaction input to transaction inputs */
	if (llist_add_node(tx->inputs, tx_in, ADD_NODE_FRONT) == -1)
		return (NULL);

	/* Create coinbase transaction output */
	tx_out = tx_out_create(COINBASE_AMOUNT, pub);
	if (!tx_out)
		return (NULL);

	/* Add coinbase transaction output to transaction outputs */
	if (llist_add_node(tx->outputs, tx_out, ADD_NODE_FRONT) == -1)
		return (NULL);

	/* Compute transaction ID */
	if (!transaction_hash(tx, tx->id))
		return (NULL);
	return (tx);
}
