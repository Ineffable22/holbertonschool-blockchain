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
			     "Usage: wallet_save <file_name>\n";
		return (1);
	}
	if (arg[2] != NULL)
	{
		state->code = 0;
		state->msg = "Error: too many arguments\n"
			     "Usage: wallet_save <file_name>\n";
		return (1);
	}
	return (0);
}

/**
 * save - Save a wallet
 * @arg:     Arguments
 * @session: Session struct
 *
 * Return: Nothing
 */
void save(char **arg, session_t *session)
{
	EC_KEY *key = NULL;
	uint8_t pub[EC_PUB_LEN] = {0};

	if (check(arg, &session->state))
		return;

	key = ec_create();
	if (!key)
	{
		session->state.code = 0;
		session->state.msg = "Error: cannot create key";
		return;
	}
	if (!ec_to_pub(key, pub))
	{
		session->state.code = 0;
		session->state.msg = "Error: cannot get public key";
		return;
	}
	if (!ec_save(key, arg[1]))
	{
		session->state.code = 0;
		session->state.msg = "Error: cannot save key";
		return;
	}
	printf("Public key: ");
	_print_hex_buffer(pub, EC_PUB_LEN);
	printf("saved in %s directory\n", arg[1]);
	session->state.code = 0;
	session->state.msg = "state: save";
}
