#include "transaction.h"

/**
 * tx_is_valid - Checks if a transaction input is valid
 * @utxo: Points to the unspent transaction output to check
 * @tx_in:   Points to the transaction input to check
 *
 * Return: 1 if the transaction input is valid, or 0 otherwise
 */
static int tx_is_valid(unspent_tx_out_t *utxo, tx_in_t *tx_in)
{
	if (!utxo || !tx_in)
		return (0);
	return (!(memcmp(utxo->block_hash, tx_in->block_hash,
			 SHA256_DIGEST_LENGTH) ||
		  memcmp(utxo->tx_id, tx_in->tx_id,
			 SHA256_DIGEST_LENGTH) ||
		  memcmp(utxo->out.hash, tx_in->tx_out_hash,
			 SHA256_DIGEST_LENGTH)));
}

/**
 * list_output_to_add - Adds a transaction output to a list of unspent
 * @out:   Points to the transaction output to add
 * @index: Is the index of the transaction output in the list
 * @data:  Points to a structure containing the list of unspent transaction
 *         outputs and the hash of the block containing the transaction list
 * Return: 0 if the transaction output is valid, or 1 otherwise
 */
static int list_output_to_add(tx_out_t *out, unsigned int index,
			      check_block_t *data)
{
	unspent_tx_out_t *utxo;

	(void)index;
	if (!out || !data)
		return (1);
	utxo = unspent_tx_out_create(data->block_hash, data->tx_id, out);
	if (!utxo)
		return (1);
	if (llist_add_node(data->all_unspent, utxo, ADD_NODE_REAR))
		return (1);
	return (0);
}

/**
 * list_input_to_del - Deletes a transaction input from a list of unspent
 * @input:  Points to the transaction input to add
 * @index:  Is the index of the transaction input in the list
 * @unspent: Points to the list of unspent transaction outputs
 *
 * Return: 0 if the transaction input is valid, or 1 otherwise
 */
static int list_input_to_del(tx_in_t *input, unsigned int index,
			     llist_t *unspent)
{
	uint8_t *hash;
	int i, end, hashes_zeroed;

	(void)index;
	if (!input || !unspent)
		return (1);
	hash = input->block_hash;
	end = SHA256_DIGEST_LENGTH * 2;
	for (i = 0, hashes_zeroed = 1; i < end; i++)
		hashes_zeroed &= (hash[i] == 0);
	/* coinbase tx inputs by definition have no referenced outputs */
	if (hashes_zeroed)
		return (0);
	if (llist_remove_node(unspent, (node_ident_t)tx_is_valid, input, 1, NULL))
		return (1);
	return (0);
}

/**
 * list_tx - Iterates over a list of transactions and updates the list of
 *           unspent transaction outputs
 * @tx:     Points to the current transaction
 * @index:  Is the index of the current transaction in the list
 * @data:   Points to a structure containing the list of unspent transaction
 *          outputs and the hash of the block containing the transaction list
 *
 * Return: 0 if the transaction is valid, or 1 otherwise
 */
static int list_tx(transaction_t *tx, unsigned int index, check_block_t *data)
{
	(void)index;
	if (!tx || !data)
		return (1);
	memcpy(&(data->tx_id), &(tx->id), SHA256_DIGEST_LENGTH);
	if (llist_for_each(tx->inputs, (node_func_t)list_input_to_del,
			   data->all_unspent) ||
	    llist_for_each(tx->outputs, (node_func_t)list_output_to_add,
			   data))
		return (1);
	return (0);
}

/**
 * update_unspent - Updates the list of all unspent transaction outputs,
 *                  given a list of processed transactions
 * @transactions: Is the list of validated transactions
 * @block_hash:   Hash of the validated Block that contains the transaction
 *                list transactions
 * @all_unspent:  Is the current list of unspent transaction outputs
 *
 * Return: A new list of unspent transaction outputs or NULL on failure
 */
llist_t *update_unspent(llist_t *transactions,
			uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	check_block_t data;

	if (!transactions || !block_hash || !all_unspent)
		return (NULL);
	memcpy(&(data.block_hash), block_hash, SHA256_DIGEST_LENGTH);
	data.all_unspent = all_unspent;
	if (llist_for_each(transactions, (node_func_t)check_tx, &data))
		return (NULL);
	return (all_unspent);
}
