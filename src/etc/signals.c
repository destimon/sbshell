/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcherend <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 15:46:26 by dcherend          #+#    #+#             */
/*   Updated: 2018/08/04 15:21:11 by dcherend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/21sh.h"

t_term *g_te;

static void		restore(int sig)
{
	int			i;

	i = -1;
	if (sig == SIGINT)
	{
		g_te = init_term();
		if (g_curr_job <= 0)
		{
			ft_putchar('\n');
			show_prompt(g_te);
		}
		while (g_te->env[++i])
			free(g_te->env[i]);
		free(g_te->env);
		free(g_te);
	}
	else if (sig == SIGTSTP)
	{
		tputs(tgetstr("cl", NULL), 1, &complete);
		tputs(tgetstr("ve", NULL), 1, &complete);
		ioctl(STDERR_FILENO, TIOCSTI, "\x1A");
		reset_input_mode();
		signal(SIGTSTP, SIG_DFL);
	}
	else
	{
		reset_input_mode();
		exit(0);
	}
}

static void		getback(int sig)
{
	t_term		*ti;

	if (sig == SIGCONT)
	{
		set_input_mode();
	}
	else
	{
		tputs(tgetstr("cl", NULL), 1, &complete);
		g_te = init_term();
		show_prompt(g_te);
		free(g_te);
	}
}

void			signals(t_term *te)
{
	g_te = te;
	signal(SIGINT, &restore);
	signal(SIGTSTP, SIG_IGN);
//	signal(SIGCONT, &getback);
	signal(SIGWINCH, &getback);
	signal(SIGABRT, &restore);
	signal(SIGKILL, &restore);
	signal(SIGQUIT, &restore);
}