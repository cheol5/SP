#include "deque.h"

int	is_empty(t_deque *deque)
{
	if (deque->cnt == 0)
		return (1);
	return (0);
}


void	append(t_deque *deque, Thread data)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	node->next = 0;
	node->prev = 0;
	node->data = data;
	if (is_empty(deque))
		deque->top = node;
	else
	{
		node->prev = deque->bottom;
		deque->bottom->next = node;
	}
	deque->bottom = node;
	deque->cnt++;
}

Thread	pop_left(t_deque *deque)
{
	t_node	*ptr;
	Thread	data;

	if (is_empty(deque))
		print_emsg("Error\n");
	ptr = deque->top;
	data = ptr->data;
	if (deque->cnt == 1)
	{
		deque->top = 0;
		deque->bottom = 0;
	}
	else
	{
		deque->top = ptr->next;
		deque->top->prev = 0;
	}
	deque->cnt--;
	free(ptr);
	return (data);
}

void	append_left(t_deque *deque, Thread *data)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	node->next = 0;
	node->prev = 0;
	node->data = data;
	if (is_empty(deque))
		deque->bottom = node;
	else
	{
		node->next = deque->top;
		deque->top->prev = node;
	}
	deque->top = node;
	deque->cnt++;
}

Thread	pop(t_deque *deque)
{
	t_node	*ptr;
	Thread	data;

	if (is_empty(deque))
		print_emsg("Error\n");
	ptr = deque->bottom;
	data = ptr->data;
	if (deque->cnt == 1)
	{
		deque->top = 0;
		deque->bottom = 0;
	}
	else
	{
		deque->bottom = ptr->prev;
		deque->bottom->next = 0;
	}
	deque->cnt--;
	free(ptr);
	return (data);
}