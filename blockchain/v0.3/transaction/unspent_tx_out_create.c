#include "transaction.h"

/**
 * unspent_tx_out_create - Allocates and initializes an unspent transaction
 * output structure
 * @block_hash: Contains the hash of the Block where the referenced transaction
 *              output is located
 * @tx_id:      Contains the hash of a transaction in the Block block_hash,
 *              where the referenced transaction output is located
 * @out:        Points to the referenced transaction output
 *
 * Return: A pointer to the created unspent transaction output upon success,
 * or NULL upon failure
 */
unspent_tx_out_t *unspent_tx_out_create(
uint8_t block_hash[SHA256_DIGEST_LENGTH],
uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out)
{
	unspent_tx_out_t *utx = NULL;

	utx = malloc(sizeof(unspent_tx_out_t));
	if (!utx)
		return (NULL);
	memcpy(utx->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(utx->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	memcpy(&utx->out, out, sizeof(utx->out));
	return (utx);
}
