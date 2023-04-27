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
	if (arg[1] != NULL)
	{
		state->code = 0;
		state->msg = "Error: Path is NULL\n"
			     "Usage: load <path>\n";
		return (1);
	}
	if (arg[2] != NULL)
	{
		state->code = 0;
		state->msg = "Error: too many arguments\n"
			     "Usage: load <path>\n";
		return (1);
	}
	return (0);
}

/**
 * deserialize - Deserialize the blockchain
 * @arg: Arguments
 * @session: Session struct
 *
 * Return: Nothing
 */
void deserialize(char **arg, session_t *session)
{
	blockchain_t *blockchain = NULL;

	if (check(arg, &session->state))
		return;

	blockchain = blockchain_deserialize(arg[1]);
	if (blockchain == NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: failed to deserialize blockchain";
		return;
	}
	blockchain_destroy(session->blockchain);
	session->blockchain = blockchain;
	session->state.code = 0;
	session->state.msg = "Blockchain deserialized";
}

/**
 * start_blockchain - Start the blockchain
 * @session: Session struct
 *
 * Return: Nothing
 */
void start_blockchain(session_t *session)
{
	wallet_t *wallet;
	char *username = "Ineffable";

	session->blockchain = blockchain_deserialize("save.hlbk");
	if (session->blockchain == NULL)
		session->blockchain = blockchain_create();

	wallet = malloc(sizeof(wallet_t));
	if (wallet == NULL)
	{
		session->state.code = 1;
		session->state.msg = "Error: failed to allocate memory for wallet";
		return;
	}
	wallet->key = ec_load(username);
	if (wallet->key == NULL)
	{
		char *args[2] = {"save", username};
		save(args, session);
		session->state.code = 0;
		session->state.msg = "Error: failed to load Ineffable's key\n"
				     "Correction: Ineffable's Key created successfully";
		return;
	}
	if (!ec_to_pub(wallet->key, wallet->pub))
	{
		session->state.code = 1;
		session->state.msg = "Error: failed to extract Miguel's public key";
		return;
	}
	add_utxo(session, wallet);
	session->state.code = 0;
	session->state.msg = "state: start_blockchain";
}

/**
 * add_utxo - Add an unspent transaction output to the blockchain
 * @session: Session struct
 * @wallet:  Wallet struct
 *
 * Return: Nothing
 */
void add_utxo(session_t *session, wallet_t *wallet)
{
	tx_out_t *out;
	unspent_tx_out_t *unspent;
	llist_t *all_unspent;
	uint8_t transaction_id[SHA256_DIGEST_LENGTH];
	uint8_t block_hash[SHA256_DIGEST_LENGTH];

	sha256((int8_t *)"Block", strlen("Block"), block_hash);
	sha256((int8_t *)"Transaction", strlen("Transaction"), transaction_id);

	out = tx_out_create(1300, wallet->pub);
	if (out == NULL)
	{
		session->state.code = 1;
		session->state.msg = "Error: failed to create transaction output for Miguel";
		return;
	}

	unspent = unspent_tx_out_create(block_hash, transaction_id, out);
	_unspent_tx_out_print(unspent);
	if (unspent == NULL)
	{
		session->state.code = 1;
		session->state.msg = "Error: failed to create utxo";
		return;
	}
	all_unspent = llist_create(MT_SUPPORT_FALSE);
	llist_add_node(all_unspent, unspent, ADD_NODE_REAR);
	session->blockchain->unspent = all_unspent;
	session->wallet = wallet;
}
