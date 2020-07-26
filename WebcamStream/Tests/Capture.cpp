#include <iostream>
#include <fstream>

#include "Webcam.h"

#define XRES 1280
#define YRES 720

int main(int argc, char **argv) {

	Webcam webcam("/dev/video0");
	AVPacket *packet = av_packet_alloc();
	int count = 0;
	while(true){
		int ret = webcam.getPacket(&packet);
		if(ret < 0){
			std::cerr << "Capturing packet failed. Num: " << count << std::endl;
			break;
		}

		++count;
	}

	return 0;
}