/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 00:45:37 by fjerinic          #+#    #+#             */
/*   Updated: 2025/08/26 20:23:45 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd(void)
{
	char	path_ptr[PATH_SIZE];

	if (!getcwd(path_ptr, PATH_SIZE))
	{
		perror("Error");
		return ;
	}
	ft_printf("%s\n", path_ptr);
}
