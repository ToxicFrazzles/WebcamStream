#include <iostream>
#include <fstream>

#include "Webcam.h"
#include "Decoder.h"

#define XRES 1280
#define YRES 720

int main(int argc, char **argv) {
	Decoder decoder = Decoder("mjpeg");
	Webcam webcam("/dev/video0", XRES, YRES);
	for(int i=0; i<60; ++i)
		webcam.frame();
	auto frame = webcam.frame();

	std::ofstream image;
	image.open("frame.jpeg");

	image.write((char *) frame.data, frame.size);
	image.close();

	return 0;
}