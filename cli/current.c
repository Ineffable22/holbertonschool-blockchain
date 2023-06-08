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
			     "Usage: current\n";
		return (1);
	}
	return (0);
}

/**
 * current - Print the current wallet
 * @arg:     Arguments
 * @session: Session struct
 *
 * Return: Nothing
 */
void current(char **arg, session_t *session)
{
	uint32_t balance = 0;
	if (check(arg, &session->state))
		return;

	if (!session->wallet)
	{
		session->state.code = 0;
		session->state.msg = "Error: no wallet loaded";
		return;
	}
	else
	{
		balance = get_balance(session->blockchain->unspent, session->wallet->pub);
		session->wallet->balance = balance;
		printf("Username: %s\n", session->wallet->username);
		printf("Public key: ");
		_print_hex_buffer(session->wallet->pub, EC_PUB_LEN);
		printf("\n");
		printf("Wallet Holbie Coin: \n");
		printf("- Balance: %u\n", balance);

	}
}
