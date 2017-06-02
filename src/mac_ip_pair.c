#include <stdlib.h>
#include <strings.h>
#include "mac_ip_pair.h"

struct mac_ip_pair *
mac_ip_pair_from(unsigned char *mac_ip_buf)
{
	struct mac_ip_pair *pair;
	pair = calloc(1, sizeof(struct mac_ip_pair));

	bcopy(mac_ip_buf, pair->mac_address, 6);
	bcopy(mac_ip_buf + 6, &pair->ip_address, 4);

	return pair;
}

struct mac_ip_pair_list *
mac_ip_pair_list_new()
{
	struct mac_ip_pair_list *list;
	
	list = calloc(1, sizeof(struct mac_ip_pair_list));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	return list;
}

void
mac_ip_pair_list_clean(struct mac_ip_pair_node *node)
{
	if (node->next == NULL) {
		free(node->pair);
	} else {
		mac_ip_pair_list_clean(node->next);
		free(node->next);
		free(node->pair);
	}
}

void
mac_ip_pair_list_free(struct mac_ip_pair_list *list)
{
	if (list->head != NULL)
		mac_ip_pair_list_clean(list->head);

	free(list->head);
	free(list);
}

struct mac_ip_pair *
mac_ip_pair_list_next(struct mac_ip_pair_list *list)
{
	list->tail = list->tail->next;
	return list->tail->pair;
}

void
mac_ip_pair_list_add(struct mac_ip_pair_list *list, struct mac_ip_pair *pair)
{
	struct mac_ip_pair_node *node;

	node = calloc(1, sizeof(struct mac_ip_pair_node));
	node->pair = pair;
	node->next = NULL;

	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}

	list->size++;
}
