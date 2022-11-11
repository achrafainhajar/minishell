/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 00:11:14 by fstitou           #+#    #+#             */
/*   Updated: 2022/11/11 15:06:25 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*normal_or_qu(t_token **b, int exec, char *str)
{
	if (!(*b)->val[1])
		return (normal_expansion(b, 1));
	else
		return (dollar_qu(b, exec, str));
}

char	*dollar_qu(t_token **b, int exec, char *str)
{
	(void)exec;
	if ((*b)->val[1] == '?')
		str = ft_strjoin(str, ft_itoa(g_shell.ret), 2);
	else if ((*b)->val[1] == ' ')
		str = ft_strjoin(str, ft_strdup("$"), 2);
	else if ((*b)->val[1] == '$')
		str = ft_strjoin(str, ft_strdup("$$"), 2);
	(*b) = (*b)->next;
	return (str);
}

char	*check_end(t_token **b, char *str, int exec)
{
	if ((*b) && (*b)->e_type == DQUOTE)
	{
		str = ft_strjoin(str, expand_dollar((*b)->val, exec), 2);
		(*b) = (*b)->next;
	}
	else if ((*b) && (*b)->e_type != END)
	{
		str = ft_strjoin(str, (*b)->val, 0);
		(*b) = (*b)->next;
	}
	return (str);
}

int	jm3arg_join_simple(t_token **b, char **str, int exec)
{
	if ((*b)->e_type == DQUOTE)
		*str = ft_strjoin(*str, expand_dollar((*b)->val, exec), 2);
	else if ((*b)->e_type != END && ((*b)->e_type
			!= DOLLAR || (!exec && (*b)->next->e_type
				!= SQUOTE && (*b)->next->e_type != DQUOTE)))
		*str = ft_strjoin(*str, (*b)->val, 0);
	if ((*b)->flag == 1)
			(*b) = (*b)->next;
	else
	{
		(*b) = (*b)->next;
		return (1);
	}
	return (0);
}

void	check_redis(t_token *tmp)
{
	while (tmp && tmp->flag == 1 && tmp->e_type
		!= DQUOTE && tmp->e_type != SQUOTE)
		tmp = tmp->next;
	if (tmp && (tmp->e_type == DQUOTE || tmp->e_type == SQUOTE))
		g_shell.mik = 1;
}

char	*jme3arg(t_token **b, int exec, int ch_d, int exp)
{
	char	*str;

	str = ft_strdup("");
	if (!exec)
		check_redis(*b);
	while ((*b) && (*b)->flag == 1)
	{
		if ((*b)->e_type == DOLLAR && exec)
		{
			checkin_dollar(b, &str, ch_d);
			if (exp && (*b)->e_type != DQUOTE
				&& (*b)->e_type != SQUOTE)
			{
				(*b)->val = ft_strjoin("\2", (*b)->val, -1);
				(*b)->val = ft_strjoin((*b)->val, "\3", -1);
			}
		}
		if (jm3arg_join_simple(b, &str, exec))
			return (str);
	}
	if ((*b) && (*b)->e_type == DOLLAR && exec)
		return (normal_or_qu(b, exec, str));
	str = check_end(b, str, exec);
	return (str);
}
