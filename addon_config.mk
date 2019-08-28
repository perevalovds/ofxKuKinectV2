meta:
	ADDON_NAME = ofxKuKinectV2
	ADDON_DESCRIPTION = Addon for Kinect V2 sensor
	ADDON_AUTHOR = perevalovds
	ADDON_TAGS = "kinect" "kinectv2" "libfreenect" "libfreenect2"
	ADDON_URL = https://github.com/perevalovds/ofxKuKinectV2

vs:	
	# x64
	#ADDON_LIBS += libs/...
	ADDON_DLLS_TO_COPY += copy_to_bin/libusb-1.0.dll;copy_to_bin/turbojpeg.dll
