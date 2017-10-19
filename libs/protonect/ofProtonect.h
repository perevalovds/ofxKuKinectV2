//  ofProtonect.cpp
//
//  Created by Theodore Watson on 11/16/15


#include "ofMain.h"

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <transfer_pool.h>
#include <event_loop.h>
#include "ofRGBPacketProcessor.h"

#include "ofAppGLFWWindow.h"
#include "ofAppRunner.h"

struct KinectV2Settings {
	bool depth;
	bool rgb;
	bool register_depth_rgb;	//sync RGB and Depth images
	bool pipe_gl;		//use GL pipe. If false, uses CPU - caution, very slow frame rate
	KinectV2Settings() {
		depth = true;
		rgb = true;
		register_depth_rgb = depth && rgb;
		pipe_gl = true;
	}
	KinectV2Settings(bool depth0, bool rgb0, bool regist, bool pipe_gl0) {
		depth = depth0; 
		rgb = rgb0; 
		register_depth_rgb = regist && depth && rgb; 
		pipe_gl = pipe_gl0;
	}
};


class ofProtonect{
    public:
        ofProtonect();
    
        int openKinect(std::string serialNo, KinectV2Settings settings = KinectV2Settings());
        void updateKinect(ofPixels & rgbPixels, ofFloatPixels & depthPixels);
        int closeKinect();
    
        libfreenect2::Freenect2 & getFreenect2Instance(){
            return freenect2;
        }

		const static int depth_w = 512;
		const static int depth_h = 424;

		libfreenect2::Freenect2Device::ColorCameraParams getColorCameraParams() {
			//Based on https://github.com/hanasaan/ofxMultiKinectV2
			if (dev) return dev->getColorCameraParams();
			return libfreenect2::Freenect2Device::ColorCameraParams(); 
		}

		libfreenect2::Freenect2Device::IrCameraParams getIrCameraParams() {
			//Based on https://github.com/hanasaan/ofxMultiKinectV2
			if (dev) return dev->getIrCameraParams(); 
			return libfreenect2::Freenect2Device::IrCameraParams();			
		}
  
		const KinectV2Settings &settings() { return settings_; }
    protected:
  
        bool bOpened;
        
        libfreenect2::Freenect2 freenect2;

        libfreenect2::Freenect2Device *dev = 0;
        libfreenect2::PacketPipeline *pipeline = 0;

        libfreenect2::FrameMap frames;

        libfreenect2::Registration* registration;
        libfreenect2::SyncMultiFrameListener * listener;
        libfreenect2::Frame  * undistorted = NULL;
        libfreenect2::Frame  * registered = NULL;
        libfreenect2::Frame  * bigFrame = NULL;

		KinectV2Settings settings_;
};