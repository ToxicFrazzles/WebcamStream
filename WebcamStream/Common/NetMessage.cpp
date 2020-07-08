#include "NetMessage.h"
#include <cstring>
#include <endian.h>

int NetMessage::serialise(char *bufout) {
	int length = 0;

	bufout[length] = this->type;
	++length;
	if(this->type == MSG_TYPE_MAINTENANCE) return length;	// A maintenance packet only needs the message type field

	uint64_t netId = htobe64(this->id);
	memcpy(bufout+length, &netId, sizeof(netId));
	length += sizeof(netId);

	switch(this->type) {
		case MSG_TYPE_CONFIG:{
			bufout[length] = this->config.type;
			++length;
			int netval = htobe32(this->config.value);
			memcpy(bufout+length, &netval, sizeof(netval));
			length += sizeof(netval);
			break;
		}
		default:
		{
			break;
		}
	}

	return length;
}

void NetMessage::deserialise(char *bufin) {
	int length = 0;

	this->type = (NetMessageType)bufin[length];
	if(this->type == MSG_TYPE_MAINTENANCE) return;		// A maintenance packet is purely to let the server know the client is still around. Decoding the rest of the packet is pointless.
	++length;

	uint64_t netId = 0;
	memcpy(&netId, bufin+length, sizeof(netId));
	this->id = be64toh(netId);
	length += sizeof(netId);

	switch(this->type) {
		case MSG_TYPE_CONFIG:{
			this->config.type = (ConfigType)bufin[length];
			++length;
			int netval;
			memcpy(&netval, bufin+length, sizeof(netval));
			this->config.value = be32toh(netval);
			length += sizeof(netval);
			break;
		}
		default:
		{
			break;
		}
	}
}