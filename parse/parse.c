/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 01:14:18 by fstitou           #+#    #+#             */
/*   Updated: 2022/11/11 15:30:34 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_parse	*init_command(void)
{
	t_parse	*command;

	command = (t_parse *)f_malloc(sizeof(t_parse));
	if (!command)
		return (NULL);
	command->cmd = NULL;
	command->argv = (char **)realloc_array(NULL, ft_strdup(""));
	command->redir = NULL;
	command->error = 0;
	command->next = NULL;
	return (command);
}

t_parse	*lst_add_back_command(t_parse *lst, t_parse *new)
{
	t_parse	*tmp;

	if (!lst)
		return (new);
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (lst);
}

int	check_expantion(t_token *token)
{
	if (token && token->flag != 1 && token->e_type == DOLLAR)
		return (1);
	else
	{
		while (token && token->flag == 1)
		{
			if (token->e_type == DOLLAR && token->next->e_type
				!= SQUOTE && token->next->e_type != DQUOTE)
				return (1);
			token = token->next;
		}
	}
	return (0);
}

int	check_expantion2(t_token *token)
{
	if (token && token->flag != 1 && token->e_type == DOLLAR)
		return (1);
	else
	{
		while (token && token->flag == 1)
		{
			if (token->e_type != DOLLAR)
				return (0);
			token = token->next;
		}
	}
	return (1);
}

void	check_init_error(t_parse *command, char *value, int type)
{
	struct stat	buf;

	if (!stat(value, &buf) || type != LESS)
		command->redir = init_redir(value, type, 0);
	else if (type == LESS)
	{
		if (command->error == 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(value, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			command->error = 1;
		}
		command->redir = init_redir(value, type, 1);
	}
}

void	check_add_error(t_parse *command, char *value, int type)
{
	struct stat	buf;

	if (!stat(value, &buf) || type != LESS)
		command->redir = add_redir(command->redir, value, type, 0);
	else if (type == LESS)
	{
		if (command->error == 0)
		{
			ft_putstr_fd(": No such file or directory\n", 2);
			command->error = 1;
		}
		command->redir = add_redir(command->redir, value, type, 1);
	}
}

void	check_file_error(t_parse *command, char *value, int type, t_token *tmp)
{
	if (check_expantion(tmp) && (!ft_split2(value)[0]
		|| !ft_split2(value)[0][0] || ft_split2(value)[1]))
	{
		if (command->error == 0)
		{
			ft_putstr_fd("minishell: ambiguis redirection\n", 2);
			command->error = 1;
		}
		if (!command->redir)
			command->redir = init_redir(value, type, 1);
		else
			command->redir = add_redir(command->redir, value, type, 1);
	}
	else
	{
		if (check_expantion(tmp))
			value = ft_strdup(ft_split2(value)[0]);
		if (!command->redir)
			check_init_error(command, value, type);
		else
			check_add_error(command, value, type);
	}
}

void	parse_helper(t_token **token, t_parse *command, char *value, int type)
{
	int			exec;
	t_token		*tmp;

	exec = type;
	if ((*token)->next->e_type == END || (*token)->next->e_type == PIPE
		|| (*token)->next->e_type == GREAT || (*token)->next->e_type == LESS
		|| (*token)->next->e_type == LESSANDLESS
		|| (*token)->next->e_type == GREATANDGREAT)
	{
		if (g_shell.err == 0)
			errors(258);
		(*token) = (*token)->next;
	}
	else
	{
		type = (*token)->e_type;
		(*token) = (*token)->next;
		tmp = *token;
		value = jme3arg(token, exec, 1, 1);
		check_file_error(command, value, type, tmp);
	}
}

void	parse_commands(t_token **token, t_parse *command)
{
	char	*value;
	t_token	*tmp;

	value = NULL;
	if ((*token)->e_type == WORD || (*token)->e_type == DQUOTE
		|| (*token)->e_type == SQUOTE || (*token)->e_type == DOLLAR)
	{
		tmp = *token;
		value = jme3arg(token, 1, 2, 1);
		if (check_expantion(tmp))
			split_expansion(value, command);
		else if (!command->cmd)
			command->cmd = value;
		else
			command->argv = (char **)realloc_array(command->argv, value);
	}
	else if ((*token)->e_type == GREAT || (*token)->e_type == LESS
		|| (*token)->e_type == LESSANDLESS || (*token)->e_type == GREATANDGREAT)
	{
		if ((*token)->e_type == LESSANDLESS)
			parse_helper(token, command, value, 0);
		else
			parse_helper(token, command, value, 1);
	}
}

void	create_commands(t_token *token, t_parse **command)
{
	t_parse	*head;

	head = *command;
	if (token->e_type == PIPE)
	{
		errors(258);
		return ;
	}
	while (token)
	{
		parse_commands(&token, head);
		if (token->e_type == PIPE || token->e_type == END)
		{
			if (syntax_error(token))
				return ;
			head = add_command(head);
			head = head->next;
			token = token->next;
		}
	}
}
