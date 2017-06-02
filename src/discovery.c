#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include "discovery.h"
#include "mac_ip_pair.h"

int
discovery_parse_field(unsigned char *raw, struct discovery *parsed)
{
	const unsigned int offset = 3;

	unsigned char field_data[64];

	unsigned short field_id = raw[0] + raw[1];
	unsigned short field_size = raw[2];

	bcopy(raw + offset, field_data, field_size);

	switch (field_id) {
	case 1:
		bcopy(field_data, parsed->mac, field_size);
		break;
	case 2:
		mac_ip_pair_list_add(parsed->interface_list,
				mac_ip_pair_from(field_data));
		break;
	case 3:
		parsed->firmware = calloc(1, field_size + 1);
		bcopy(field_data, parsed->firmware, field_size);
		parsed->firmware[field_size] = '\0';
		printf("Firmware: %s\n", parsed->firmware);
		break;
	case 10:
		bcopy(field_data, &parsed->uptime, field_size);
		parsed->uptime = htonl(parsed->uptime);
		printf("Uptime (seconds): %u\n", parsed->uptime);
		break;
	case 11:
		parsed->hostname = calloc(1, field_size + 1);
		bcopy(field_data, parsed->hostname, field_size);
		parsed->hostname[field_size] = '\0';
		printf("Hostname: %s\n", parsed->hostname);
		break;
	case 12:
		parsed->model_code = calloc(1, field_size + 1);
		bcopy(field_data, parsed->model_code, field_size);
		parsed->model_code[field_size] = '\0';
		printf("Model: %s\n", parsed->model_code);
		break;
	case 13:
		parsed->ssid = calloc(1, field_size + 1);
		bcopy(field_data, parsed->ssid, field_size);
		parsed->ssid[field_size] = '\0';
		printf("SSID: %s\n", parsed->ssid);
		break;
	case 14:
		bcopy(field_data, &parsed->unknown1, field_size);
		printf("Unknown (1 byte): %u\n", parsed->unknown1);
		break;
	case 16:
		bcopy(field_data, &parsed->unknown2, field_size);
		printf("Unknown (2 bytes): %u\n", parsed->unknown2);
		break;
	case 20:
		parsed->model_name = calloc(1, field_size + 1);
		bcopy(field_data, parsed->model_name, field_size);
		parsed->model_name[field_size] = '\0';
		printf("Model Name: %s\n", parsed->model_name);
		break;
	default:
		printf("Unknown field type: %u\n", field_id);
		return -1;
	}

	return field_size + offset;
}

int
discovery_parse(unsigned char *raw, struct discovery *parsed)
{
	const unsigned int offset = 4;

	int rc;

	parsed->response = raw[0] + raw[1];
	parsed->size = raw[2] + raw[3];
	unsigned char *tmp = raw + offset;
	int nbytes = 0;

	while (nbytes < parsed->size) {
		rc = discovery_parse_field(tmp, parsed);
		if (rc < 0) break;

		nbytes += rc;
		tmp += rc;
	}


	if (nbytes == parsed->size)
		return nbytes;
	else
		return -1;
}

struct discovery *
discovery_new_from(unsigned char *raw)
{
	int rc;
	struct discovery *parsed;
	
	parsed = calloc(1, sizeof(struct discovery));
	parsed->interface_list = mac_ip_pair_list_new();
	rc = discovery_parse(raw, parsed);

	if (rc < 0) {
		discovery_free(parsed);
		return NULL;
	} else {
		return parsed;
	}
}

void
discovery_free(struct discovery *data)
{
	mac_ip_pair_list_free(data->interface_list);
	free(data->hostname);
	free(data->model_code);
	free(data->ssid);
	free(data->firmware);
	free(data->model_name);
}
