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
		state->msg = "Error: missing filename\n"
			     "Usage: wallet_save <filename>\n";
		return (1);
	}
	if (arg[2] != NULL)
	{
		state->code = 0;
		state->msg = "Error: too many arguments\n"
			     "Usage: wallet_save <filename>\n";
		return (1);
	}
	return (0);
}

void save_file(char **arg, session_t *session)
{
	// EC_KEY *key = NULL;
	// uint8_t pub[EC_PUB_LEN] = {0};

	if (check(arg, &session->state))
		return;

	
}