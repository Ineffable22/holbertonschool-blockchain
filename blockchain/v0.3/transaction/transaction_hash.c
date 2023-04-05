#include "transaction.h"

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
	uint8_t *buf = NULL;

	if (!transaction || !hash_buf)
		return (NULL);
	len_in = llist_size(transaction->inputs) * 3;
	len_out = llist_size(transaction->outputs);
	if (!len_in || !len_out)
		return (NULL);
	
	buf = malloc((len_in + len_out) * SHA256_DIGEST_LENGTH);
	if (!sha256((int8_t *)buf,
		    (len_in + len_out) * SHA256_DIGEST_LENGTH, hash_buf))
		return (NULL);
	free(buf);
	return (hash_buf);
}
