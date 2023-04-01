#include "transaction.h"

/**
 * tx_in_create - Allocates and initializes a transaction input structure
 * @unspent: Points to the unspent transaction output to be converted
 *           to a transaction input
 *
 * Return: A pointer to the created transaction input upon success,
 *         or NULL upon failure
 */
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *tx_in = NULL;

	if (!unspent)
		return (NULL);
	tx_in = malloc(sizeof(tx_in_t));
	if (!tx_in)
		return (NULL);
	memcpy(tx_in->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(tx_in->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(tx_in->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);
	memset(tx_in->sig.sig, 0, SIG_MAX_LEN);
	tx_in->sig.len = 0;
	return (tx_in);
}
