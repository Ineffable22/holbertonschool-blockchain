#include "cli.h"

/**
 * get_balance - Get the balance of a wallet
 * @all_unspent: List of all unspent transaction outputs
 * @pub_sender:  Public key of the sender
 *
 * Return: The balance of the wallet
 */
uint32_t get_balance(llist_t *all_unspent, uint8_t pub_sender[EC_PUB_LEN])
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
	return (unspend);
}