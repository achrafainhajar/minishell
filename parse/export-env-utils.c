/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export-env-utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aainhaja <aainhaja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 22:31:07 by aainhaja          #+#    #+#             */
/*   Updated: 2022/11/10 01:54:50 by aainhaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../minishell.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

t_env	*lst_new(char *key, char sep, char *val)
{
	t_env	*new;

	new = f_malloc(sizeof(t_env));
	new->key = key;
	new->val = val;
	new->sep = sep;
	new->next = NULL;
	return (new);
}

void	lst_add_backenv(t_env **lst, t_env *new)
{
	t_env	*tmp;

	tmp = *lst;
	if (!new)
		return ;
	new->next = NULL;
	if (!*lst)
		*lst = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	init_env(char **env)
{
	char	*key;
	char	*val;
	int		i;
	char	**tmp;

	i = 0;
	g_shell.ev = NULL;
	while (env && env[i])
	{
		tmp = ft_split(env[i], '=');
		key = tmp[0];
		val = tmp[1];
		lst_add_backenv(&g_shell.ev, lst_new(key, '=',
				val));
		i++;
	}
}

char	*my_getenv(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = (env);
	while (tmp)
	{
		if (strcmp(tmp->key, key) == 0)
			return (tmp->val);
		tmp = tmp->next;
	}
	return (NULL);
}
