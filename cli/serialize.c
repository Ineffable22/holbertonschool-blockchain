#include "cli.h"

/**
 * check - Check if the arguments are valid
 * @arg:      Arguments
 * @session:  Session struct
 *
 * Return: 0 if valid, 1 if not
 */
int check(char **arg, session_t *session)
{
	if (arg[1] != NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: too many arguments\n"
				     "Usage: save <path>\n";
		return (0);
	}
	if (session->blockchain == NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: blockchain not loaded";
		return (0);
	}
	if (session->wallet == NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: wallet not loaded";
		return (0);
	}
	if (llist_size(session->tx_pool) == 0)
	{
		session->state.code = 0;
		session->state.msg = "Error: transaction pool empty";
		return (0);
	}
	return (1);
}

/**
 * serialize - Saves the blockchain
 * @arg:      Arguments
 * @session:  Session struct
 *
 * Return: Nothing
 */
void serialize(char **arg, session_t *session)
{
	if (check(arg, session) == 0)
		return;
	if (blockchain_serialize(session->blockchain, BACKUP))
	{
		session->state.code = 0;
		session->state.msg = "Error: Failed to save blockchain";
		return;
	}
	session->state.code = 0;
	session->state.msg = "state: save";
}
