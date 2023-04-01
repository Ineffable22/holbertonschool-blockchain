#include "transaction.h"

/**
 * transaction_create - Creates a transaction
 * @sender:      Contains the private key of the transaction sender
 * @receiver:    Contains the public key of the transaction receiver
 * @amount:      Is the amount to send
 * @all_unspent: Is the list of all the unspent outputs to date
 *
 * Return: A pointer to the created transaction upon success,
 *         or NULL upon failure
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
				  uint32_t amount, llist_t *all_unspent)
{
	transaction_t *tx = NULL;
	uint8_t sender_pub[EC_PUB_LEN], receiver_pub[EC_PUB_LEN];
	uint8_t block_hash[SHA256_DIGEST_LENGTH];

	if (!sender || !receiver || !all_unspent)
		return (NULL);

	/* Get the public key from the private key */
	if (!ec_to_pub(sender, sender_pub) || !ec_to_pub(receiver, receiver_pub))
		return (NULL);

	/* Create a new transaction */
	tx = malloc(sizeof(transaction_t));
	if (!tx)
		return (NULL);
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!tx->inputs || !tx->outputs)
	{
		llist_destroy(tx->inputs, 1, NULL);
		free(tx);
		return (NULL);
	}
	memcpy(tx->id, transaction_hash(tx, block_hash), SHA256_DIGEST_LENGTH);
	if (!tx->id)
	{
		llist_destroy(tx->inputs, 1, NULL);
		llist_destroy(tx->outputs, 1, NULL);
		free(tx);
		return (NULL);
	}
	(void)amount;

	return (tx);
}
