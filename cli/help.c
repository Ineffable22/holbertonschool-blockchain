#include "cli.h"

/**
 * help - Print the help message
 * @arg:     Arguments
 * @session: Session struct
 *
 * Return: Nothing
 */
void help(char **arg, session_t *session)
{
	if (arg[1] != NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: too many arguments";
		return;
	}
	session->state.code = 0;
	session->state.msg = "state: help";
	printf("Usage: \n");
	printf("wallet_load [file] \n");
	printf("wallet_save [file] \n");
}
