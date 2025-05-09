#include "shell.h"

/**
 * main - Entry point of the simple shell program
 *
 * Description: This function implements a simple shell that:
 * - Displays a prompt ($)
 * - Reads user input
 * - Parses command and arguments
 * - Handles PATH
 * - Implements exit and env built-ins
 * - Executes commands using execve
 * - Handles errors and EOF condition
 * - Continues until the user types exit or EOF (Ctrl+D)
 * @argc: Number of arguments
 * @argv: Array of arguments
 *
 * Return: Always 0 (Success)
 */
int main(int argc, char *argv[])
{

	char *progname;
	int cmd_count = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *args[MAX_ARGS];
	int arg_count;
	int builtin_result;
	int is_interactive = isatty(STDIN_FILENO);
	int last_status = 0;
	(void)argc;

	progname = argv[0];

	while (1)
	{
		if (is_interactive)
		{
			write(STDOUT_FILENO, "$ ", 2); /* Display prompt when interactive */
			fflush(stdout);
		}
		nread = getline(&line, &len, stdin); /* Read user input */
		if (nread == -1)
		{
			if (is_interactive)
				printf("\n");
			break;
		}
		if (line[nread - 1] == '\n') /* Remove newline character */
			line[nread - 1] = '\0';
		if (strlen(line) == 0) /* Skip empty lines */
			continue;
		arg_count = parse_input(line, args); /* Parse command and arguments */
		if (arg_count == 0) /* Skip if no command was entered */
			continue;
		cmd_count++;
		builtin_result = handle_builtins(args); /* Handle built-in commands */
		if (builtin_result == 2) /* exit command */
		{
			free(line);
			return (last_status); /* Return the last exit code */
		}
		if (builtin_result == 1) /* other built-in handled */
			continue;
		last_status = execute_command(args, progname, cmd_count); /* Store the exit code */
	}
	free(line);
	return (last_status); /* Return the exit code of the last executed command */
}
