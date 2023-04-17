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
 * tx_in_sign - Signs a transaction input, given the transaction id it is from
 * @in:          Points to the transaction input structure to sign
 * @tx_id:       Contains the ID (hash) of the transaction the transaction
 *               input to sign is stored in
 * @sender:      Contains the private key of the receiver of the coins
 *               contained in the transaction output referenced by the
 *               transaction input
 * @all_unspent: Is the list of all unspent transaction outputs to date
 *
 * Return: A pointer to the resulting signature structure upon success,
 *         or NULL upon failure
 */
signature_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
		  EC_KEY const *sender, llist_t *all_unspent)
{
	uint8_t sender_pub[EC_PUB_LEN];
	unspent_tx_out_t *utx_out;

	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);

	/* Get the public key from the private key */
	if (!ec_to_pub(sender, sender_pub))
		return (NULL);

	/* Check that the transaction input is valid */
	utx_out = llist_find_node(all_unspent, (node_ident_t)tx_is_valid, in);
	if (!utx_out)
		return (NULL);

	/* Check that the transaction input matches the sender */
	if (memcmp(utx_out->out.pub, sender_pub, EC_PUB_LEN))
		return (NULL);
	/* Sign the transaction input */
	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &(in->sig)))
		return (NULL);
	return (&(in->sig));
}
