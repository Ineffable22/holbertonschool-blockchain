#include "transaction.h"

/**
 * match_unspent - Matches a transaction input with an unspent transaction
 * @node: Points to an unspent transaction output
 * @arg:  Points to a transaction input
 *
 * Return: 1 if the transaction input matches the unspent transaction output,
 * and 0 otherwise
 */
int match_unspent(llist_node_t node, void *arg)
{
	unspent_tx_out_t *utxo = node;
	tx_in_t *txi = arg;

	if (!memcmp(txi->tx_out_hash, utxo->out.hash, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}

/**
 * check_output - Check the output
 * @node: Points to the transaction output to verify
 * @idx:  Is the index of the node in the list
 * @arg:  Is a pointer to a structure containing the transaction to verify
 *
 * Return: 1 if the transaction output is valid, and 0 otherwise
 */
int check_output(llist_node_t node, unsigned int idx, void *arg)
{
	tx_out_t *txo = node;
	tx_info_t *info = arg;
	(void)idx;

	info->out_amount += txo->amount;
	return (0);
}

/**
 * check_input - Checks whether a transaction input is valid
 * @node: Points to the transaction input to verify
 * @idx: Is the index of the node in the list
 * @arg: Is a pointer to a structure containing the transaction to verify
 *
 * Return: 1 if the transaction input is valid, and 0 otherwise
 */
int check_input(llist_node_t node, unsigned int idx, void *arg)
{
	tx_in_t *txi = node;
	tx_info_t *info = arg;
	unspent_tx_out_t *utxo =
	    llist_find_node(info->all_unspent, match_unspent, txi);
	EC_KEY *key;

	(void)idx;
	if (!utxo)
	{
		info->bool = 0;
		return (1);
	}
	key = ec_from_pub(utxo->out.pub);
	if (!key ||
	    !ec_verify(key, info->tx->id, SHA256_DIGEST_LENGTH, &txi->sig))
	{
		info->bool = 0;
		return (EC_KEY_free(key), 1);
	}
	EC_KEY_free(key);
	info->in_amount += utxo->out.amount;
	return (0);
}

/**
 * transaction_is_valid - Checks whether a transaction is valid
 * @transaction: Points to the transaction to verify
 * @all_unspent: Is the list of all unspent transaction outputs to date
 *
 * Return: 1 if the transaction is valid, and 0 otherwise
 */
int transaction_is_valid(transaction_t const *transaction,
			 llist_t *all_unspent)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];
	tx_info_t info = {0};

	if (!transaction || !all_unspent)
		return (0);
	if (!transaction_hash(transaction, hash))
		return (0);
	if (memcmp(transaction->id, hash, SHA256_DIGEST_LENGTH))
		return (0);
	info.all_unspent = all_unspent;
	info.tx = transaction;
	info.bool = 1;
	if (llist_for_each(transaction->inputs, check_input, &info) ||
	    !info.bool)
		return (0);
	if (llist_for_each(transaction->outputs, check_output, &info) ||
	    info.in_amount != info.out_amount || !info.in_amount)
		return (0);
	return (1);
}
