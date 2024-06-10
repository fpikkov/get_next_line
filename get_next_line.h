/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpikkov <fpikkov@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:23:16 by fpikkov           #+#    #+#             */
/*   Updated: 2024/06/10 14:11:34 by fpikkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2
# endif
# include <unistd.h>
# include <stdlib.h>

typedef struct s_list
{
	char			*content;
	struct s_list	*next;

}				t_list;

t_list	*new_list(void);
void	append_list(t_list **head, char *content);
t_list	*last_list(t_list **head);
int		find_nl(t_list **head);
int		store_buff(t_list **head, int fd);
int		line_len(t_list **head);
char	*next_line(t_list **head);
char	*fetch_remainder(t_list **head);
char	*clear_list(t_list **head, int err);
char	*get_next_line(int fd);

#endif
