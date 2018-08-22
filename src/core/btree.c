#include "../../inc/21sh.h"

static t_btree	*alloc_btree(void *value)
{
	t_btree		*btree;

	btree = (t_btree*)malloc(sizeof(t_btree));
	if (btree)
	{
		btree->data = value;
		btree->parent = NULL;
		btree->left = NULL;
		btree->right = NULL;
		return (btree);
	}
	return (NULL);
}

static t_btree	*get_root(char **pipes)
{
	t_btree		*btree;

	btree = alloc_btree((void*)'|');
	if (btree && pipes && pipes[0] && pipes[1])
	{
		btree->left = alloc_btree((void*)pipes[0]);
		btree->right = alloc_btree((void*)pipes[1]);
		return (btree);
	}
	return (NULL);
}

t_btree			*catch_pipes(char *cmd)
{
	char		**pipes;
	t_btree		*root;
	t_btree		*top;
	t_btree		*tmp;
	int			i;

	i = 2;
	pipes = ft_strsplit(cmd, '|');
	root = get_root(pipes);
	if (root)
	{
		tmp = root;
		while (pipes[i])
		{
			top = alloc_btree((void*)'|');
			if (top)
			{
				top->right = alloc_btree(pipes[i]);
				top->left = tmp;
//				printf("top->right->data: %s\n", (char*)top->right->data);
//				printf("top->left->data: %s\n", (char*)top->left->right->data);
				tmp = top;
			}
			i++;
		}
		return (tmp);
	}
}