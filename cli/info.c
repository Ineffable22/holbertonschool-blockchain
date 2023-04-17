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
			     "Usage: mine\n";
		return (1);
	}
	return (0);
}

/**
 * info - Prints the info of the blockchain
 * @arg:     Arguments
 * @session: Session struct
 *
 * Return: Nothing
 */
void info(char **arg, session_t *session)
{
	if (check(arg, &session->state))
		return;

	if (session->blockchain == NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: blockchain not loaded";
		return;
	}
	if (session->wallet == NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: wallet not loaded";
		return;
	}
	if (llist_size(session->tx_pool) == 0)
	{
		session->state.code = 0;
		session->state.msg = "Error: transaction pool empty";
		return;
	}
	printf("Blockchain: \n");
	printf("\tchain: %d\n", llist_size(session->blockchain->chain));
	printf("\tUnspent: %d\n", llist_size(session->blockchain->unspent));
	printf("Transaction pool: %d transactions\n",
	       llist_size(session->tx_pool) < 0 ? 0 : llist_size(session->tx_pool));

	session->state.code = 0;
	session->state.msg = "Success";
}
