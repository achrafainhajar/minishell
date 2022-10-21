#include"../minishell.h"

void	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (s)
	{
		while (s[i])
		{
			write(1, &s[i], 1);
			i++;
		}
	}
}
void simple_print(t_parse *cmd)
{
	int i;
    i = 0;

    while(cmd->argv && cmd->argv[i])
    {
        ft_putstr(cmd->argv[i]);
        i++;
		if (cmd->argv[i])
			write(1, " ", 1);
    }
    ft_putstr("\n");
}
int check_n(char *str)
{
	int i;

	i = 0;
	if(str[i] != '-')
		return(0);
	i++;
	while(str[i])
	{
		if(str[i] != 'n')
			return(0);
		i++;
	}
	return(1);
}
void n_print(t_parse *cmd)
{
	int i;

	i = 1;
	while(cmd->argv[i])
	{
		if(check_n(cmd->argv[i]) == 0)
		{
			ft_putstr(cmd->argv[i]);
		}
		i++;
		if (cmd->argv[i])
			write(1, " ", 1);
	}
}
void ft_echo(t_parse *cmd)
{
	if(check_n(cmd->argv[0]) == 1)
		n_print(cmd);
	else
	{
		simple_print(cmd);
	}
}