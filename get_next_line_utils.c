/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpikkov <fpikkov@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:28:01 by fpikkov           #+#    #+#             */
/*   Updated: 2024/06/10 14:19:25 by fpikkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Returns the last node in the linked list
t_list	*last_list(t_list **head)
{
	t_list	*temp;

	if (!(*head))
		return (NULL);
	temp = *head;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

// Looks for newline characters at the end of the linked list
int	find_nl(t_list **head)
{
	t_list	*last;
	int		i;

	i = 0;
	last = last_list(head);
	if (!last)
		return (0);
	while (last->content[i] != '\0')
	{
		if (last->content[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

// Finds the amount of characters to allocate until the newline character
int	line_len(t_list **head)
{
	t_list	*temp;
	int		len;
	int		i;

	len = 0;
	if (*head == NULL)
		return (0);
	temp = *head;
	while (temp != NULL)
	{
		i = 0;
		while (temp->content[i] != '\0')
		{
			if (temp->content[i] == '\n')
			{
				len++;
				break ;
			}
			i++;
			len++;
		}
		temp = temp->next;
	}
	return (len);
}

// Returns any remaining characters after newline
char	*fetch_remainder(t_list **head)
{
	t_list	*last;
	char	*buffer;
	int		i;
	int		j;

	last = last_list(head);
	if (!last)
		return (NULL);
	i = 0;
	while (last->content[i] != '\0' && last->content[i] != '\n')
		i++;
	if (last->content[i] == '\n')
		i++;
	if (last->content[i] == '\0')
		return (NULL);
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	j = 0;
	while (last->content[i] != '\0')
		buffer[j++] = last->content[i++];
	buffer[j] = '\0';
	return (buffer);
}

// Stores any remaining chars after newline in buffer, then frees old list
// Function has a stupid return of NULL to reduce GNL's line count
char	*clear_list(t_list **head, int err)
{
	char	*buffer;
	t_list	*current;
	t_list	*next;

	if (!(*head))
		return (NULL);
	buffer = NULL;
	if (err == 0)
		buffer = fetch_remainder(head);
	current = *head;
	next = current;
	while (next != NULL)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	*head = NULL;
	if (buffer)
	{
		append_list(head, buffer);
		free(buffer);
	}
	return (NULL);
}
