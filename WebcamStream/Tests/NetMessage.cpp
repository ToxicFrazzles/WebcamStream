#include <iostream>
#include <cstring>
#include "NetMessage.h"

bool test_message(NetMessage *testmsg){
	// Serialising portion
	char *buffer = new char[1024];
	testmsg->serialise(buffer);

	// Deserialising portion
	bool success = true;
	auto *msg = new NetMessage();
	msg->deserialise(buffer);


	success &= (msg->type == testmsg->type);
	if(!success){
		std::cout << "Failed at NetMessage.type" << std::endl;
		return success;
	}
	switch(msg->type){
		case  MSG_TYPE_MAINTENANCE:
			break;
		case MSG_TYPE_UNKNOWN:
			success &= (msg->id == testmsg->id);
			if(!success){
				std::cout << "Failed at NetMessage.id" << std::endl;
				return success;
			}
			break;
		case MSG_TYPE_CONFIG:
			success &= (msg->id == testmsg->id);
			if(!success){
				std::cout << "Failed at NetMessage.id" << std::endl;
				return success;
			}
			success &= (msg->config.type == testmsg->config.type);
			if(!success){
				std::cout << "Failed at NetMessage.config.type " << +msg->config.type << " != " << +testmsg->config.type << std::endl;
				return success;
			}
			success &= (msg->config.value == testmsg->config.value);
			if(!success){
				std::cout << "Failed at NetMessage.config.value" << std::endl;
				return success;
			}
			break;
		case MSG_TYPE_VIDEO:
			break;
	}

	delete msg;
	delete [] buffer;
	return success;
}

int main(int argc, char *argv[]){
	auto *msg = new NetMessage();
	bool success = true;

	msg->type = MSG_TYPE_UNKNOWN;
	msg->id = UINT64_MAX;
	success &= test_message(msg);

	msg->type = MSG_TYPE_MAINTENANCE;
	success &= test_message(msg);

	msg->type = MSG_TYPE_CONFIG;
	msg->config.type = CONFIG_TYPE_VIDEO;
	msg->config.value = 65536;
	success &= test_message(msg);

	if(success){
		std::cout << "(De)Serialising network message objects is WORKING" << std::endl;
	}else{
		std::cout << "(De)Serialising network message objects is NOT WORKING" << std::endl;
	}
	delete msg;
	return 0;
}