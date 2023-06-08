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

	blockchain = blockchain_deserialize(BACKUP);
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
	wallet_t *wallet = NULL;
	char *username = "Ineffable";

	session->blockchain = blockchain_deserialize(BACKUP);
	if (session->blockchain == NULL)
		session->blockchain = blockchain_create();

	wallet = calloc(1, sizeof(wallet_t));
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
	memcpy(wallet->pub, ec_to_pub(wallet->key, wallet->pub), EC_PUB_LEN);
	if (!wallet->pub)
	{
		session->state.code = 1;
		session->state.msg = "Error: failed to extract Miguel's public key";
		return;
	}
	add_utxo(session, wallet);
	session->state.code = 0;
	session->state.msg = "state: start_blockchain";
	session->wallet->username = username;
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
	tx_out_t *out = NULL;
	unspent_tx_out_t *unspent = NULL;
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
	free(out);
	_unspent_tx_out_print(unspent);
	if (unspent == NULL)
	{
		session->state.code = 1;
		session->state.msg = "Error: failed to create utxo";
		return;
	}
	llist_add_node(session->blockchain->unspent, unspent, ADD_NODE_REAR);
	if (session->wallet)
	{
		free(session->wallet);
		EC_KEY_free(session->wallet->key);
	}
	session->wallet = wallet;
}
