#pragma once

struct discovery {
	struct mac_ip_pair_list *interface_list;
	char *hostname;
	char *model_code;
	char *ssid;
	char *firmware;
	char *model_name;
	int uptime;		// in seconds
	short response;
	unsigned short size;
	char mac[6];
	short unknown2;
	char unknown1;
};

int discovery_parse_field(unsigned char *, struct discovery *);
int discovery_parse(unsigned char *, struct discovery *);
struct discovery * discovery_new_from(unsigned char *);
void discovery_free(struct discovery *);
