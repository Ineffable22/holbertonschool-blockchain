#include "transaction.h"

/**
 * tx_out_create - Allocates and initializes a transaction output structure
 * @amount: Is the amount of the transaction
 * @pub:    Is the public key of the transaction receiver
 *
 * Return: A pointer to the created transaction output upon success,
 * or NULL upon failure
 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *tx_out = NULL;

	tx_out = malloc(sizeof(tx_out_t));
	if (!tx_out)
		return (NULL);
	tx_out->amount = amount;
	memcpy(tx_out->pub, pub, EC_PUB_LEN);
	if (!sha256((int8_t const *)tx_out, SHA256_DIGEST_LENGTH,
		    tx_out->hash))
	{
		free(tx_out);
		return (NULL);
	}
	return (tx_out);
}
