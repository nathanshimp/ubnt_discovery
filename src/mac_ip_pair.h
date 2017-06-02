#pragma once

struct mac_ip_pair {
	char mac_address[6];
	int ip_address;
};

struct mac_ip_pair_node {
	struct mac_ip_pair *pair;
	struct mac_ip_pair_node *next;
};

struct mac_ip_pair_list {
	struct mac_ip_pair_node *head;
	struct mac_ip_pair_node *tail;
	int size;
};

struct mac_ip_pair * mac_ip_pair_from(unsigned char *);
struct mac_ip_pair_list * mac_ip_pair_list_new();
void mac_ip_pair_list_free(struct mac_ip_pair_list *);
void mac_ip_pair_list_clean(struct mac_ip_pair_node *);
struct mac_ip_pair * mac_ip_pair_list_next(struct mac_ip_pair_list *);
void mac_ip_pair_list_add(struct mac_ip_pair_list *, struct mac_ip_pair *);
