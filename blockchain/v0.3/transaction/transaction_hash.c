#include "transaction.h"

/**
 * hash_inputs - llist action func to hash inputs
 * @node: tx_in_t * struct
 * @idx: index of node
 * @arg: pointer to address to write to
 * Return: 0 if success else 1
 */
static int hash_inputs(llist_node_t node, unsigned int idx, void *arg)
{
	(void)idx;
	memcpy(*(uint8_t **)arg, node, SHA256_DIGEST_LENGTH * 3);
	*(uint8_t **)arg += SHA256_DIGEST_LENGTH * 3;
	return (0);
}

/**
 * hash_outputs - llist action func to hash outputs
 * @node: tx_out_t * struct
 * @idx: index of node
 * @arg: pointer to address to write to
 * Return: 0 if success else 1
 */
static int hash_outputs(llist_node_t node, unsigned int idx, void *arg)
{
	(void)idx;
	memcpy(*(uint8_t **)arg, ((tx_out_t *)node)->hash, SHA256_DIGEST_LENGTH);
	*(uint8_t **)arg += SHA256_DIGEST_LENGTH;
	return (0);
}

/**
 * transaction_hash - Computes the ID (hash) of a transaction
 * @transaction: Points to the transaction to compute the hash of
 * @hash_buf:    Is a buffer in which to store the computed hash
 *
 * Return: A pointer to hash_buf
 */
uint8_t *transaction_hash(transaction_t const *transaction,
			  uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	uint32_t len_in = 0, len_out = 0;
	uint8_t *buf = NULL, *_buf = NULL;

	if (!transaction || !hash_buf)
		return (NULL);
	len_in = llist_size(transaction->inputs) * 3;
	len_out = llist_size(transaction->outputs);
	if (!len_in || !len_out)
		return (NULL);

	buf = malloc((len_in + len_out) * SHA256_DIGEST_LENGTH);
	_buf = buf;
	llist_for_each(transaction->inputs, hash_inputs, &buf);
	llist_for_each(transaction->outputs, hash_outputs, &buf);
	if (!sha256((const int8_t *)buf,
		    (len_in + len_out) * SHA256_DIGEST_LENGTH, hash_buf))
		return (NULL);
	free(_buf);
	return (hash_buf);
}
