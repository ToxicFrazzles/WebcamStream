#ifndef WEBCAMSTREAM_WEBCAM2_H
#define WEBCAMSTREAM_WEBCAM2_H

#include <string>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
}

class Webcam2 {
	std::string devName;
	AVInputFormat *ifmt = nullptr;
	AVDictionary *options = nullptr;
	AVFormatContext *ifmt_ctx = nullptr;
	AVCodecContext *video_dec_ctx = nullptr;
	AVCodecContext *video_enc_ctx = nullptr;
	AVPacket *pkt = nullptr;
	AVFrame *frame = nullptr;
	int video_frame_count = 0;
	int video_stream_idx = -1;
	AVStream *video_stream = nullptr;
public:
	explicit Webcam2(std::string device);
	static bool hasCodec(const std::string &device, uint32_t fourcc);
	static bool hasCodec(struct v4l2_fmtdesc *formats, int formatCount, uint32_t fourcc);
	static int listFormats(const std::string &device, struct v4l2_fmtdesc **formats);
	int getPacket(AVPacket **packet);
};

#endif //WEBCAMSTREAM_WEBCAM2_H
