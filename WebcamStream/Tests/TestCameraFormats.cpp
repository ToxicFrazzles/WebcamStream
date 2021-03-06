#include <linux/videodev2.h>
#include <Webcam.h>
#include <iostream>

int main()
{
	struct v4l2_fmtdesc *formats = nullptr;
	int formatCount = Webcam::listFormats("/dev/video0", &formats);

	std::cout << "The webcam supports the following formats:" << std::endl;
	for(int i=0; i<formatCount; ++i){
		std::cout << "\t" << formats[i].description << std::endl;
	}
	std::cout << std::endl;

	if(Webcam::hasCodec(formats, formatCount, v4l2_fourcc('H', '2', '6', '4'))){
		std::cout << "Webcam has H.264 hardware encoding! This will make streaming easier due to 0 transcoding." << std::endl;
	}else if(Webcam::hasCodec(formats, formatCount, v4l2_fourcc('M', 'J', 'P', 'G'))){
		std::cout << "Webcam doesn't have H.264 hardware encoding, but it does have Motion-JPEG so we can still get high framerates. (Assuming we can transcode fast enough)" << std::endl;
	}else{
		std::cout << "The webcam doesn't support hardware encoding into H.264 or Motion-JPEG. This would cause low framerates." << std::endl;
	}
}