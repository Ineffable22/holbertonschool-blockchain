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
	printf("\twallet_load <file_name>\n");
	printf("\twallet_save <file_name>\n");
	printf("\tcurrent\n");
	printf("\tsend <amount> <address>\n");
	printf("\tmine\n");
	printf("\tinfo\n");
	printf("\tload\n");
	printf("\tsave\n");
	printf("\thelp\n");
	printf("\texit\n");
}
