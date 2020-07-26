#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#include "Webcam.h"

Webcam::Webcam(std::string device)
{
	devName = std::move(device);
	av_register_all();
	avdevice_register_all();
	avcodec_register_all();

	struct v4l2_fmtdesc *formats = (struct v4l2_fmtdesc *)reallocarray(nullptr, 1,
																	   sizeof(v4l2_fmtdesc));        // Allocate just enough memory to reallocate it inside the listFormats method
	int formatCount = Webcam::listFormats(devName, &formats);

	ifmt = av_find_input_format("v4l2");
	av_dict_set(&options, "framerate", "30", 0);
	av_dict_set(&options, "video_size", "1920x1080", 0);
	if(Webcam::hasCodec(formats, formatCount, v4l2_fourcc('H', '2', '6', '4')))
		av_dict_set(&options, "input_format", "h264", 0);
	else
		av_dict_set(&options, "input_format", "mjpeg", 0);

	if(avformat_open_input(&ifmt_ctx, devName.c_str(), ifmt, &options) != 0) {
		std::cerr << "Failed to open video source" << std::endl;
		exit(1);
	}

	if(avformat_find_stream_info(ifmt_ctx, nullptr) < 0) {
		std::cerr << "Failed to find the stream info" << std::endl;
	}


	int ret = 0;

	// dump input information to stderr
	av_dump_format(ifmt_ctx, 0, devName.c_str(), 0);

	frame = av_frame_alloc();
	pkt = av_packet_alloc();
	video_dec_ctx = avcodec_alloc_context3(avcodec_find_decoder(ifmt_ctx->streams[0]->codecpar->codec_id));
	video_enc_ctx = avcodec_alloc_context3(avcodec_find_encoder(AV_CODEC_ID_H264));
}


int Webcam::getPacket(AVPacket **outPacket)
{
	static bool alerted = false;
	int ret;
	if((ret = av_read_frame(ifmt_ctx, pkt)) < 0){
		return ret;
	}
	if(ifmt_ctx->streams[pkt->stream_index]->codecpar->codec_id == video_dec_ctx->codec_id){
		*outPacket = pkt;
		if(!alerted){
			std::cout << "Input and output formats are the same so we don't need to transcode. Just copying the packet is fine." << std::endl;
			alerted = true;
		}
	}else{
		avcodec_send_packet(video_dec_ctx,pkt);
		avcodec_receive_frame(video_dec_ctx, frame);
		avcodec_send_frame(video_enc_ctx, frame);
		avcodec_receive_packet(video_enc_ctx, pkt);
		*outPacket = pkt;
		if(!alerted){
			std::cout << "Input and output formats are different so we need to transcode." << std::endl;
			alerted = true;
		}
	}
	return ret;
}


bool Webcam::hasCodec(const std::string &device, uint32_t fourcc)
{
	bool result = false;

	struct v4l2_fmtdesc *formats = nullptr;
	int formatCount = Webcam::listFormats("/dev/video0", &formats);
	for(int i = 0; i < formatCount; ++i) {
		if(formats[i].pixelformat == fourcc) {
			result = true;
			break;
		}
	}
	free(formats);		// Free the memory that was allocated by listFormats
	return result;
}

bool Webcam::hasCodec(struct v4l2_fmtdesc *formats, int formatCount, uint32_t fourcc)
{
	bool result = false;
	for(int i = 0; i < formatCount; ++i) {
		if(formats[i].pixelformat == fourcc) {
			result = true;
			break;
		}
	}
	return result;
}

int Webcam::listFormats(const std::string &device, struct v4l2_fmtdesc **formats)
{
	int count = 0;        // Keep track of how many format structs have been stored in the memory region
	int space = 10;        // Keep track of how many format structs can be stored in the memory region

	int fd = open(device.c_str(), O_RDWR);        // Open the webcam file
	if(fd != -1)        // If it was successfullly opened
	{

		// Make sure we have enough space for most formats
		*formats = static_cast<struct v4l2_fmtdesc *>(reallocarray(*formats, space, sizeof(v4l2_fmtdesc)));

		struct v4l2_fmtdesc fmtdesc;            // Temporary storage of the current format being read
		memset(&fmtdesc, 0, sizeof(fmtdesc));
		fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		while(ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc) == 0) {    // If there is a format at the format index
			// Copy the format into the dynamic array
			memcpy(&(*formats)[count], &fmtdesc, sizeof(v4l2_fmtdesc));

			fmtdesc.index++;
			++count;
			if(count == space) {        // If we have run out of space to store formats
				// Allocate enough space for 5 more
				space += 5;
				*formats = static_cast<struct v4l2_fmtdesc *>(reallocarray(*formats, space, sizeof(v4l2_fmtdesc)));
			}
		}
		close(fd);        // Close the webcam
	}
	return count;
}
