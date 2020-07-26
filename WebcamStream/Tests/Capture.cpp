#include <iostream>
#include <fstream>
#include <ctime>

#include "Webcam.h"

int main(int argc, char **argv) {

	Webcam webcam("/dev/video0");
	AVPacket *packet = av_packet_alloc();
	int count = 0;
	time_t clock = time(nullptr);
	while(true){
		int ret = webcam.getPacket(&packet);
		if(ret < 0){
			std::cerr << "Capturing packet failed." << std::endl;
			break;
		}
		++count;
		double seconds = difftime(time(nullptr), clock);
		if(seconds >= 1){
			double framerate = (double)count/seconds;
			std::cout << "Capturing at " << framerate << " Frames/Second" << std::endl;
			clock = time(nullptr);
			count = 0;
		}

	}
	av_packet_free(&packet);
	return 0;
}