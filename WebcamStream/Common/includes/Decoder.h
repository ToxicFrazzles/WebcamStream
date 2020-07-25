#ifndef WEBCAMSTREAM_DECODER_H
#define WEBCAMSTREAM_DECODER_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <string>

class Decoder {
	const AVCodec *codec;
	AVCodecContext *c;
public:
	explicit Decoder(const std::string& name);
	void decode(AVPacket *inpkt, AVFrame *outframe);
};

#endif //WEBCAMSTREAM_DECODER_H
