#include "cli.h"

/**
 * check - Check if the arguments are valid
 * @arg:   Arguments
 * @state: Error state
 *
 * Return: 0 if valid, 1 if not
 */
static int32_t check(char **arg, error_t *state)
{
	if (arg[1] == NULL)
	{
		state->code = 0;
		state->msg = "Error: amount is NULL\n"
			     "Usage: send <amount> <receiver>";
		return (1);
	}
	if (arg[2] == NULL)
	{
		state->code = 0;
		state->msg = "Error: receiver is NULL\n"
			     "Usage: send <amount> <receiver>";
		return (1);
	}
	if (arg[3] != NULL)
	{
		state->code = 0;
		state->msg = "Error: too many arguments\n"
			     "Usage: send <amount> <receiver>";
		return (1);
	}
	return (0);
}

/**
 * send - Send an amount of coins to a receiver
 * @arg:     Arguments
 * @session: Session struct
 *
 * Return: Nothing
 */
void send(char **arg, session_t *session)
{
	int32_t amount = 0;
	EC_KEY *receiver = NULL;
	transaction_t *tx = NULL;
	uint8_t pub[EC_PUB_LEN] = {0};

	if (session->wallet == NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: wallet is NULL";
		return;
	}

	if (check(arg, &session->state))
		return;
	amount = atoi(arg[1]);
	if (amount <= 0)
	{
		session->state.code = 0;
		session->state.msg = "Error: amount must be greater than 0";
		return;
	}
	receiver = get_receiver(arg, session, receiver, pub);
	if (!receiver)
		return;
	create_utxo(session, receiver, amount);
	EC_KEY_free(receiver);
	printf("Amount: %d\n", amount);
	printf("Receiver: ");
	_print_hex_buffer(pub, EC_PUB_LEN);
	printf("\n");
	printf("Sender: ");
	_print_hex_buffer(session->wallet->pub, EC_PUB_LEN);
	printf("\n");
	printf("Transaction: ");
	_print_hex_buffer(tx->id, SHA256_DIGEST_LENGTH);
	printf("\n");
	session->state.code = 0;
	session->state.msg = "state: send";
}

/**
 * get_receiver - Get the receiver's public key
 * @arg:      Arguments
 * @session:  Session struct
 * @receiver: Receiver's public key
 * @pub:      Receiver's public key
 *
 * Return: Receiver's public key
 */
EC_KEY *get_receiver(char **arg, session_t *session, EC_KEY *receiver,
		     uint8_t pub[EC_PUB_LEN])
{
	int32_t i = 0;

	for (i = 0; i < EC_PUB_LEN; i++)
	{

		char hex_byte_str[2] = {arg[2][i * 2], arg[2][i * 2 + 1]};

		pub[i] = (uint8_t)strtol(hex_byte_str, NULL, 16);
	}

	receiver = ec_from_pub(pub);
	if (!receiver)
	{
		session->state.code = 0;
		session->state.msg = "Error: can not get public key";
		return (NULL);
	}
	return (receiver);
}

/**
 * create_utxo - Create a new unspent transaction output
 * @session:  Session struct
 * @receiver: Receiver's public key
 * @amount:   Amount of coins to send
 *
 * Return: Nothing
 */
void create_utxo(session_t *session, EC_KEY *receiver, int32_t amount)
{
	transaction_t *tx = NULL;

	tx = transaction_create(session->wallet->key, receiver, amount,
				session->blockchain->unspent);
	if (!tx)
	{
		session->state.code = 0;
		session->state.msg = "Error: can not create transaction";
		return;
	}
	if (!transaction_is_valid(tx, session->blockchain->unspent))
	{
		session->state.code = 0;
		session->state.msg = "Error: transaction is invalid";
		return;
	}
	if (!session->tx_pool)
	{
		session->tx_pool = llist_create(MT_SUPPORT_TRUE);
		if (!session->tx_pool)
		{
			session->state.code = 0;
			session->state.msg = "Error: can not create transaction pool";
			return;
		}
	}

	if (llist_add_node(session->tx_pool, tx, ADD_NODE_REAR))
	{
		session->state.code = 0;
		session->state.msg = "Error: can not add transaction to pool";
		return;
	}
}
