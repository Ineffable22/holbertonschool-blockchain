#include "cli.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	int32_t i = 0;
	char *buffer = NULL;
	size_t size = 0;
	session_t session = {0};

	signal(SIGINT, response_signal);
	start_blockchain(&session);
	if (session.state.msg)
	{
		fprintf(stdout, "%s\n", session.state.msg);
		session.state.msg = NULL;
	}
	while (i != EOF)
	{
		if (isatty(STDIN_FILENO))
			write(1, "> ", 2);
		i = getline(&buffer, &size, stdin);
		if (i > 1)
			parse_str(buffer, &session);
		if (session.state.msg)
		{
			fprintf(stdout, "%s\n", session.state.msg);
			session.state.msg = NULL;
		}
		if (session.state.code != 0)
			break;
	}
	blockchain_destroy(session.blockchain);
	llist_destroy(session.tx_pool, 0, free);
	free(session.wallet);
	free(buffer);
	return (session.state.code > 0 ? session.state.code : 0);
}

/**
 * response_signal - Prints the prompt signal (Ctrl + C)
 * @x: Unused number
 *
 * Return: Nothing
 */
void response_signal(int x)
{
	char *str = "\n> ";

	(void)x;
	write(1, str, 3);
}

/**
 * parse_str - Parse the string
 * @buffer:  String to parse
 * @session: Session struct
 *
 * Return: Nothing
 */
void parse_str(char *buffer, session_t *session)
{
	char *arg[] = {NULL, NULL, NULL, NULL};
	char *delim = " \t\r\n";

	arg[0] = strtok(buffer, delim);
	if (arg[0] == NULL)
	{
		session->state.code = 1;
		session->state.msg = "Error: token is NULL";
		return;
	}
	arg[1] = strtok(NULL, delim);
	arg[2] = strtok(NULL, delim);
	arg[3] = strtok(NULL, delim);
	if (arg[3] != NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: too many arguments";
		return;
	}
	search_cmd(arg, session);
}

/**
 * search_cmd - Search the command
 * @arg:     Arguments
 * @session: Session struct
 *
 * Return: Nothing
 */
void search_cmd(char **arg, session_t *session)
{
	int32_t i = 0;
	cmd_t cmd[] = {
	    {"wallet_load", load},
	    {"wallet_save", save},
	    {"current", current},
	    {"send", send},
	    {"mine", mine},
	    {"info", info},
	    {"load", deserialize},
	    {"save", serialize},
	    {"help", help},
	    {"exit", exit_cli},
	    {NULL, NULL},
	};

	while (cmd[i].cmd != NULL)
	{
		if (strcmp(arg[0], cmd[i].cmd) == 0)
		{
			cmd[i].f(arg, session);
			return;
		}
		i++;
	}
	session->state.code = 0;
	session->state.msg = "Error: command not found";
}

/**
 * exit_cli - Exit the CLI
 * @arg:     Arguments
 * @session: Session struct
 *
 * Return: Nothing
 */
void exit_cli(char **arg, session_t *session)
{
	if (arg[1] != NULL)
	{
		session->state.code = 0;
		session->state.msg = "Error: too many arguments";
		return;
	}
	session->state.code = -1;
	session->state.msg = "state: exit";
}
