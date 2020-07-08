#ifndef WEBCAMSTREAM_NETMESSAGE_H
#define WEBCAMSTREAM_NETMESSAGE_H

#include <cstdint>

enum NetMessageType : uint8_t {
	MSG_TYPE_MAINTENANCE,		// Essentially a keep-alive packet. Very quick to check if a packet is of this type before deserialising anything else
	MSG_TYPE_CONFIG,			// Configuration message. Used for subscribing to events, negotiating link settings etc.
	MSG_TYPE_VIDEO,				// Video frame message
	MSG_TYPE_UNKNOWN=255		// Unknown message type. Used in testing.
};

enum ConfigType : uint8_t {
	CONFIG_TYPE_NONE,
	CONFIG_TYPE_VIDEO,
};

struct Config {
	ConfigType type;
	int value;
};

class NetMessage {
public:
	// Universal fields present in all messages
	NetMessageType type = MSG_TYPE_UNKNOWN;
	uint64_t id = 0;

	// Config type fields
	struct Config config = {
		.type = CONFIG_TYPE_NONE,
		.value = 0,
	};

	int serialise(char *bufout);
	void deserialise(char *bufin);
};


#endif //WEBCAMSTREAM_NETMESSAGE_H
