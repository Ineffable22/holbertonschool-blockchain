#include "transaction.h"

/**
 * validate_money - validate if the sender has enough money to send
 * @amount:      Amount to send
 * @all_unspent: List of all unspent transactions
 * @pub_sender:  Public key of the sender
 *
 * Return: The amount of money the sender has
 */
uint32_t validate_money(uint32_t amount, llist_t *all_unspent,
			uint8_t pub_sender[EC_PUB_LEN])
{
	unspent_tx_out_t *txo = NULL;
	uint32_t i = 0, len = llist_size(all_unspent);
	uint32_t unspend = 0;

	for (i = 0; i < len; i++)
	{
		txo = llist_get_node_at(all_unspent, i);
		if (!txo || memcmp(txo->out.pub, pub_sender, EC_PUB_LEN))
			continue;
		unspend += txo->out.amount;
	}
	if (unspend < amount)
	{
		fprintf(stderr, "Not enough money to send this transaction amount\n");
		return (0);
	}
	return (unspend);
}

/**
 * create_utxos - create the transaction outputs
 * @amount:       Amount to send
 * @pub_sender:   Public key of the sender
 * @pub_receiver: Public key of the receiver
 * @tx:           Transaction to add the outputs
 * @unspend:      Amount of money the sender has
 * Return: The transaction with the outputs
 */
transaction_t *create_utxos(uint32_t amount,
			    uint8_t pub_sender[EC_PUB_LEN], uint8_t pub_receiver[EC_PUB_LEN],
			    transaction_t *tx, uint32_t unspend)
{
	tx_out_t *txo_receiver = NULL, *txo_sender = NULL;

	if (unspend > amount)
	{
		txo_sender = tx_out_create(unspend - amount, pub_sender);
		if (!txo_sender)
			return (NULL);
	}
	txo_receiver = tx_out_create(amount, pub_receiver);
	if (!txo_receiver)
		return (NULL);

	if (llist_add_node(tx->outputs, txo_receiver, ADD_NODE_REAR) ||
	    (txo_sender && llist_add_node(tx->outputs, txo_sender, ADD_NODE_REAR)))
		return (NULL);

	return (tx);
}

/**
 * sign_inputs - sign the inputs of the transaction
 * @tx:	         Transaction to sign
 * @sender:      Private key of the sender
 * @all_unspent: List of all unspent transactions
 *
 * Return: 0 on success, 1 on failure
 */
int sign_inputs(transaction_t *tx, EC_KEY const *sender, llist_t *all_unspent)
{
	tx_in_t *txi = NULL;
	uint32_t i = 0, len = llist_size(tx->inputs);

	if (!transaction_hash(tx, tx->id))
		return (0);
	for (i = 0; i < len; i++)
	{
		txi = llist_get_node_at(tx->inputs, i);
		if (!tx_in_sign(txi, tx->id, sender, all_unspent))
			return (0);
	}
	return (1);
}

/**
 * discount_money - discount the amount of money from the sender
 * @all_unspent: List of all unspent transactions
 * @amount:      Amount to send
 * @tx:          Transaction to add the inputs
 * @pub_sender:  Public key of the sender
 * Return: 0 on success, 1 on failure
 */
int discount_money(llist_node_t all_unspent, uint32_t amount,
		   transaction_t *tx, uint8_t pub_sender[EC_PUB_LEN])
{
	uint32_t i = 0, len = llist_size(all_unspent), unspent = 0;
	unspent_tx_out_t *txo = NULL;
	tx_in_t *txi = NULL;

	for (i = 0; i < len; i++)
	{
		txo = llist_get_node_at(all_unspent, i);
		if (!txo)
			return (0);
		if (memcmp(txo->out.pub, pub_sender, EC_PUB_LEN))
			continue;
		unspent += txo->out.amount;
		txi = tx_in_create(txo);
		if (!txi)
			return (0);
		if (llist_add_node(tx->inputs, txi, ADD_NODE_REAR))
			return (0);
		if (unspent >= amount)
			break;
	}
	return (1);
}

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
	uint8_t pub_sender[EC_PUB_LEN], pub_receiver[EC_PUB_LEN];
	uint32_t unspend = 0;

	/* Check parameters */
	if (!sender || !receiver || !all_unspent || amount <= 0)
		return (NULL);

	if (!ec_to_pub(sender, pub_sender) || !ec_to_pub(receiver, pub_receiver))
		return (NULL);

	/* Valitate if sender has enough money */
	unspend = validate_money(amount, all_unspent, pub_sender);
	if (!unspend)
		return (free(tx), NULL);

	/* Create transaction */
	tx = calloc(1, sizeof(*tx));
	if (!tx)
		return (NULL);

	/* Create transaction inputs outputs */
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!tx->inputs || !tx->outputs)
		return (free(tx), NULL);

	/* Discount amount from sender */
	if (!discount_money(all_unspent, amount, tx, pub_sender))
		return (free(tx), NULL);

	/* Create transaction output */
	tx = create_utxos(amount, pub_sender, pub_receiver, tx, unspend);
	if (!tx)
		return (NULL);
	/* Sign transaction */
	if (!sign_inputs(tx, sender, all_unspent))
		return (free(tx), NULL);
	return (tx);
}
