/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcherend <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 13:30:11 by dcherend          #+#    #+#             */
/*   Updated: 2018/08/06 15:08:36 by dcherend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/21sh.h"

#define CMD 0
#define F_CMP(x, y) ft_strcmp(x, y)

static void		commands_switch(t_term *te, char **query)
{
	g_curr_job = -1;
	if (!query[0])
		return ;
	if (F_CMP(query[CMD], "help") == 0)
		help_out();
	else if (F_CMP(query[CMD], ".") == 0 || F_CMP(query[CMD], "..") == 0 ||
			 F_CMP(query[CMD], "/") == 0 || F_CMP(query[CMD], "~") == 0)
		cmd_cd(te, &query[0]);
	else if (F_CMP(query[CMD], "echo") == 0)
		cmd_echo(te, &query[1]);
	else if (F_CMP(query[CMD], "cd") == 0)
		cmd_cd(te, &query[1]);
	else if (F_CMP(query[CMD], "env") == 0)
		cmd_env(te, &query[1]);
	else if (F_CMP(query[CMD], "setenv") == 0)
		cmd_setenv(te, &query[1]);
	else if (F_CMP(query[CMD], "unsetenv") == 0)
		cmd_unsetenv(te, &query[1]);
	else if (F_CMP(query[CMD], "exit") == 0)
		exit(0);
	else if (cmd_exec(te, query[0], &query[0]) == 0)
		unknown_cmd(query[CMD]);
}

static void		replacement(t_term *te, char **string)
{
	int			i;
	char		*var;

	i = 0;
	while (string[i])
	{
		if (string[i][0] == '~')
		{
			var = string[i];
			string[i] = tilde_handler(te, string[i]);
			free(var);
		}
		else if (string[i][0] == '$' && string[i][1])
		{
			var = var_namecut(&string[i][1]);
			free(string[i]);
			string[i] = get_var(te, var);
			free(var);
		}
		else if (string[i][0] == '"')
		{
			var = string[i];
			string[i] = remove_occ(string[i], '"');
			free(var);
		}
		else if (string[i][0] == '\'')
		{
			var = string[i];
			string[i] = remove_occ(string[i], '\'');
			free(var);
		}
		i++;
	}
}

void			commands_space(t_term *te, char *input)
{
	char	**query;
	int		i;

	if (!input)
		return ;
	query = ft_strsplit_two(input, ' ', '\t');
	free(input);
	if (!ft_elems(query))
	{
		free(query);
		return ;
	}
	replacement(te, query);
	commands_switch(te, query);
	i = 0;
	while (query[i])
	{
		ft_strdel(&query[i]);
		i++;
	}
	free(query);
}