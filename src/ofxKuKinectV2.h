//
//  ofxKuKinectV2.cpp
//
//  Addon for working with Kinect V2.
//  Modified by Kuflex
//	Original is created by Theodore Watson on 6/23/2014.
//
//  Note: in Windows it may be necessary to include this header AFTER some other headers to obtain success compilation


#pragma once

#include "ofProtonect.h"
#include "ofMain.h"

class ofxKuKinectV2 : public ofThread{

    public:
        struct KinectDeviceInfo{
            string serial;
            int deviceId;   //if you have the same devices plugged in device 0 will always be the same Kinect
            int freenectId; //don't use this one - this is the index given by freenect2 - but this can change based on order device is plugged in
        };

        ofxKuKinectV2();
        ~ofxKuKinectV2(); 
        
        //for some reason these can't be static - so you need to make a tmp object to query them
        vector <KinectDeviceInfo> getDeviceList();
        unsigned int getNumDevices();
    
        bool open(string serial, KinectV2Settings settings = KinectV2Settings());
        bool open(unsigned int deviceId = 0, KinectV2Settings settings = KinectV2Settings());
        void update();
        void close();

		const KinectV2Settings &settings() { return protonect.settings(); }
    
        bool isFrameNew();
    
        ofPixels &getDepthPixels(float mindist=500, float maxdist=6000, int mirror_x=0);
        ofPixels &getRgbPixels();
        ofFloatPixels &getRawDepthPixels();
		ofShortPixels &getShortDepthPixels();	//loss of precision, consumes CPU power

		static const int depth_w = ofProtonect::depth_w;
		static const int depth_h = ofProtonect::depth_h;
    
		ofProtonect &getProtonect() { return protonect; }

		float getDistanceAt(int x, int y);
		ofVec3f getWorldCoordinateAt(int x, int y);
		ofVec3f getWorldCoordinateAt(int x, int y, float z);
		void convertScreenToWorld(const vector<ofPoint> &pnt, vector<ofPoint> &pnt_world);

		const vector<ofPoint> &getPointCloud(int area_x = 0, int area_y = 0, int area_w = depth_w, int area_h = depth_h, float area_dist0 = -1, float area_dist1 = -1);
		//area_dist0,1 filters points by distance, if area_dist0,1 < 0, then no distance control

		//get points inside parallelepiped
		const vector<ofPoint> &getPointCloudInsideVolume(ofPoint corner0 = ofPoint(-10000,-10000,-10000), ofPoint corner1=ofPoint(10000,10000,10000),
			int mirror_x = 0, int mirror_y = 0);


		float fps() { return fps_; }

		//use getIrCameraParams() for recalculating depth -> 3D cloud without kinect library,
		//see formula in getWorldCoordinateAt()
		void getIrCameraParams(float &cx, float &cy, float &fx, float &fy);


    protected:
        void threadedFunction();

        ofPixels rgbPix;
        ofPixels depthPix;
        ofFloatPixels rawDepthPixels;
		ofShortPixels shortDepthPixels;

		vector<ofPoint> pnt;
    
        bool bNewBuffer;
        bool bNewFrame;
        bool bOpened;
    
        ofProtonect protonect; 
    
        ofPixels rgbPixelsBack;
        ofPixels rgbPixelsFront;
        ofFloatPixels depthPixelsBack;
        ofFloatPixels depthPixelsFront;
        int lastFrameNo; 

		float fps_;
		float time_;
};