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
		state->msg = "Error: missing file name\n"
			     "Usage: wallet_load <file_name>\n";
		return (1);
	}
	if (arg[2] != NULL)
	{
		state->code = 0;
		state->msg = "Error: too many arguments\n"
			     "Usage: wallet_load <file_name>\n";
		return (1);
	}
	return (0);
}

/**
 * load - Load a wallet
 * @arg:     Arguments
 * @session: Session struct
 *
 * Return: Nothing
 */
void load(char **arg, session_t *session)
{
	EC_KEY *key = NULL;
	uint8_t pub[EC_PUB_LEN] = {0};
	uint32_t balance = 0;

	if (check(arg, &session->state))
		return;
	key = ec_load(arg[1]);
	if (!key)
	{
		session->state.code = 0;
		session->state.msg = "Error: cannot load key";
		save(arg, session);
		return;
	}
	if (!ec_to_pub(key, pub))
	{
		session->state.code = 0;
		session->state.msg = "Error: cannot get public key";
		return;
	}
	printf("Public key loaded: ");
	_print_hex_buffer(pub, EC_PUB_LEN);
	putchar('\n');
	
	session->state.code = 0;
	session->state.msg = "session loaded";

	if (!session->wallet)
		session->wallet = calloc(1, sizeof(wallet_t));
	memcpy(session->wallet->pub, pub, EC_PUB_LEN);
	if (session->wallet->key)
		EC_KEY_free(session->wallet->key);
	session->wallet->key = key;
	session->wallet->username = arg[1];
	balance = get_balance(session->blockchain->unspent, session->wallet->pub);
	session->wallet->balance = balance;
}
