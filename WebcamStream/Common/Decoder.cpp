#include "Decoder.h"
#include <iostream>

Decoder::Decoder(const std::string& name)
{
	avcodec_register_all();
	codec = avcodec_find_decoder_by_name(name.c_str());
	if(!codec){
		std::cerr << "Codec not found " << name << std::endl;
		exit(1);
	}
	c = avcodec_alloc_context3(codec);
	avcodec_open2(c, codec, nullptr);
}

void Decoder::decode(AVPacket *inpkt, AVFrame *outframe)
{
	avcodec_send_packet(c, inpkt);
	avcodec_receive_frame(c, outframe);
}