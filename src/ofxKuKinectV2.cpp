//
//  ofxKuKinectV2.cpp
//
//  Midified by Kuflex, 10/2017
//	Original is created by Theodore Watson on 6/23/2014.
//

#include "ofxKuKinectV2.h"

//--------------------------------------------------------------------------------
ofxKuKinectV2::ofxKuKinectV2(){
    bNewFrame  = false;
    bNewBuffer = false;
    bOpened    = false;
    lastFrameNo = -1;

	fps_ = 0;
	time_ = -1;
    
}

//--------------------------------------------------------------------------------
ofxKuKinectV2::~ofxKuKinectV2(){
    close();
}

//--------------------------------------------------------------------------------
static bool sortBySerialName( const ofxKuKinectV2::KinectDeviceInfo & A, const ofxKuKinectV2::KinectDeviceInfo & B ){
    return A.serial < B.serial;
}

//--------------------------------------------------------------------------------
vector <ofxKuKinectV2::KinectDeviceInfo> ofxKuKinectV2::getDeviceList(){
    vector <KinectDeviceInfo> devices;
    
    int num = protonect.getFreenect2Instance().enumerateDevices();
    for (int i = 0; i < num; i++){
        KinectDeviceInfo kdi;
        kdi.serial = protonect.getFreenect2Instance().getDeviceSerialNumber(i);
        kdi.freenectId = i; 
        devices.push_back(kdi);
    }
    
    ofSort(devices, sortBySerialName);
    for (int i = 0; i < num; i++){
        devices[i].deviceId = i;
    }
    
    return devices;
}

//--------------------------------------------------------------------------------
unsigned int ofxKuKinectV2::getNumDevices(){
   return getDeviceList().size(); 
}

//--------------------------------------------------------------------------------
bool ofxKuKinectV2::open(unsigned int deviceId, KinectV2Settings settings){
    
    vector <KinectDeviceInfo> devices = getDeviceList();
    
    if( devices.size() == 0 ){
        ofLogError("ofxKuKinectV2::open") << "no devices connected!";
        return false;
    }
    
    if( deviceId >= devices.size() ){
        ofLogError("ofxKuKinectV2::open") << " deviceId " << deviceId << " is bigger or equal to the number of connected devices " << devices.size() << endl;
        return false;
    }

    string serial = devices[deviceId].serial;
    return open(serial, settings);
}

//--------------------------------------------------------------------------------
bool ofxKuKinectV2::open(string serial, KinectV2Settings settings){
    close(); 
    
    bNewFrame  = false;
    bNewBuffer = false;
    bOpened    = false;
    
    int retVal = protonect.openKinect(serial, settings);
    
    if(retVal==0){
        lastFrameNo = -1;
        startThread(true);
    }else{
        return false;
    }
    
    bOpened = true;
    return true;
}

//--------------------------------------------------------------------------------
void ofxKuKinectV2::threadedFunction(){

    while(isThreadRunning()){
        protonect.updateKinect(rgbPixelsBack, depthPixelsBack);
		if (settings().rgb) rgbPixelsFront.swap(rgbPixelsBack);
		if (settings().depth) depthPixelsFront.swap(depthPixelsBack);
                
		//fps update
		float time = ofGetElapsedTimef();
		if (time_ >= 0) {
			float delta = time - time_;
			if (delta > 0) {
				float fps = 1.0 / delta;
				fps_ += (fps - fps_)*0.3;
			}
		}
		time_ = time;

        lock();
        bNewBuffer = true;
        unlock();
    }
}

//--------------------------------------------------------------------------------
void ofxKuKinectV2::update(){
    if( ofGetFrameNum() != lastFrameNo ){
        bNewFrame = false;
        lastFrameNo = ofGetFrameNum();
    }
    if( bNewBuffer ){
    
        lock();
		if (settings().rgb) rgbPix = rgbPixelsFront;
		if (settings().depth)rawDepthPixels = depthPixelsFront;
        bNewBuffer = false;
        unlock();
        
        bNewFrame = true; 
    }
}

//--------------------------------------------------------------------------------
bool ofxKuKinectV2::isFrameNew(){
    return bNewFrame; 
}

//--------------------------------------------------------------------------------
ofPixels &ofxKuKinectV2::getDepthPixels(float mindist, float maxdist, int mirror_x){
	if (rawDepthPixels.size() > 0) {
		if (depthPix.getWidth() != rawDepthPixels.getWidth()) {
			depthPix.allocate(rawDepthPixels.getWidth(), rawDepthPixels.getHeight(), 1);
		}
		float * pixelsF = rawDepthPixels.getData();
		unsigned char * pixels = depthPix.getData();

		for (int i = 0; i < depthPix.size(); i++) {
			pixels[i] = ofMap(rawDepthPixels[i], mindist, maxdist, 255, 0, true);
			if (pixels[i] == 255) {
				pixels[i] = 0;
			}
		}

		//by default, image is mirrored
		if (!mirror_x) {
			int w = depthPix.getWidth();
			int h = depthPix.getHeight();
			int w1 = w - 1;
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w/2; x++) {
					swap(pixels[x + w * y], pixels[w1 - x + w * y]);
				}
			}
		}
	}

    return depthPix;
}

//--------------------------------------------------------------------------------
ofFloatPixels &ofxKuKinectV2::getRawDepthPixels(){
    return rawDepthPixels;
}

//--------------------------------------------------------------------------------
ofShortPixels &ofxKuKinectV2::getShortDepthPixels() { 	//loss of precision, consumes CPU power
	if (rawDepthPixels.getWidth() == depth_w && rawDepthPixels.getHeight() == depth_h) {
		if (shortDepthPixels.getWidth() != depth_w || shortDepthPixels.getHeight() != depth_h) {
			shortDepthPixels.allocate(depth_w, depth_h, 1);
		}
		float *data_f = rawDepthPixels.getData();
		unsigned short *data_int = shortDepthPixels.getData();
		for (int i = 0; i < depth_w * depth_h; i++) {
			data_int[i] = data_f[i];
		}
	}
	return shortDepthPixels;
}

//--------------------------------------------------------------------------------
ofPixels &ofxKuKinectV2::getRgbPixels(){
    return rgbPix; 
}

//--------------------------------------------------------------------------------
void ofxKuKinectV2::close(){
    if( bOpened ){
        waitForThread(true);
        protonect.closeKinect();
        bOpened = false;
    }
}

//--------------------------------------------------------------------------------
float ofxKuKinectV2::getDistanceAt(int x, int y) {
	//Based on https://github.com/hanasaan/ofxMultiKinectV2
	if (rawDepthPixels.isAllocated()) {
		return rawDepthPixels[x + y * rawDepthPixels.getWidth()];
	}
	return 0.0f;
}

//--------------------------------------------------------------------------------
ofVec3f ofxKuKinectV2::getWorldCoordinateAt(int x, int y) {
	//Based on https://github.com/hanasaan/ofxMultiKinectV2
	// TODO: use undistorted
	return getWorldCoordinateAt(x, y, getDistanceAt(x, y));
}


//--------------------------------------------------------------------------------
//use getIrCameraParams() for recalculating depth -> 3D cloud without kinect library,
//see formula in getWorldCoordinateAt()
void ofxKuKinectV2::getIrCameraParams(float &cx, float &cy, float &fx, float &fy) {
	if (bOpened) {
		libfreenect2::Freenect2Device::IrCameraParams p = protonect.getIrCameraParams();
		cx = p.cx;
		cy = p.cy;
		fx = p.fx;
		fy = p.fy;
	}
	else {
		cx = cy = fx = fy = 0;
	}
}

//--------------------------------------------------------------------------------
//https://github.com/hanasaan/ofxMultiKinectV2
ofVec3f ofxKuKinectV2::getWorldCoordinateAt(int x, int y, float z) {
	//Based on https://github.com/hanasaan/ofxMultiKinectV2
	ofVec3f world;
	if (bOpened) {
		libfreenect2::Freenect2Device::IrCameraParams p = protonect.getIrCameraParams();
		world.z = z;
		world.x = (x - p.cx) * z / p.fx;
		world.y = -(y - p.cy) * z / p.fy;
	}
	return world;
}

//--------------------------------------------------------------------------------
void ofxKuKinectV2::convertScreenToWorld(const vector<ofPoint> &pnt, vector<ofPoint> &pnt_world) {
	if (!bOpened) {
		pnt_world.clear();
		return;
	}
	libfreenect2::Freenect2Device::IrCameraParams param = protonect.getIrCameraParams();
	int n = pnt.size();
	pnt_world.resize(n);
	for (int i = 0; i < n; i++) {
		const ofPoint &in = pnt[i];
		ofPoint &out = pnt_world[i];
		out.z = in.z;
		out.x = (in.x - param.cx) * in.z / param.fx;
		out.y = -(in.y - param.cy) * in.z / param.fy;
	}
}

//--------------------------------------------------------------------------------
const vector<ofPoint> &ofxKuKinectV2::getPointCloud(int area_x, int area_y, int area_w, int area_h, float area_dist0, float area_dist1) {
	pnt.clear();
	if (!bOpened) return pnt;

	libfreenect2::Freenect2Device::IrCameraParams param = protonect.getIrCameraParams();
	if (bOpened && rawDepthPixels.getWidth() == depth_w && rawDepthPixels.getHeight() == depth_h) {

		//We optimize this function to achieve faster performance
		//TODO: use undistorted ? (see getWorldCoordinateAt())

		if (area_dist0 < 0 && area_dist1 < 0) {
			//no distance filter			
			pnt.resize(area_w * area_h);
			int k = 0;
			for (int y = area_y; y < area_y + area_h; y++) {
				for (int x = area_x; x < area_x + area_w; x++) {
					float z = rawDepthPixels[x + depth_w * y];
					ofVec3f &p = pnt[k++];
					p.z = z;
					p.x = (x - param.cx) * z / param.fx;
					p.y = -(y - param.cy) * z / param.fy;
				}
			}
		}
		else {
			//distance filter
			pnt.resize(depth_w * depth_h);
			int k = 0;
			ofVec3f p;
			for (int y = area_y; y < area_y + area_h; y++) {
				for (int x = area_x; x < area_x + area_w; x++) {
					float z = rawDepthPixels[x + depth_w * y];
					if (z >= area_dist0 && z <= area_dist1) {
						p.z = z;
						p.x = (x - param.cx) * z / param.fx;
						p.y = -(y - param.cy) * z / param.fy;
						pnt[k++] = p;
					}
				}
			}
			pnt.resize(k);
		}
	}
	return pnt;
}


//--------------------------------------------------------------------------------
//get points inside parallelepiped
const vector<ofPoint> &ofxKuKinectV2::getPointCloudInsideVolume(ofPoint corner0, ofPoint corner1, int mirror_x, int mirror_y) {
	pnt.clear();
	if (!bOpened) return pnt;

	libfreenect2::Freenect2Device::IrCameraParams param = protonect.getIrCameraParams();
	if (bOpened && rawDepthPixels.getWidth() == depth_w && rawDepthPixels.getHeight() == depth_h) {

		//We optimize this function to achieve faster performance
		//TODO: use undistorted ? (see getWorldCoordinateAt())

		float koef_fx = 1.0 / param.fx;
		float koef_fy = 1.0 / param.fy;
		if (mirror_x) koef_fx = -koef_fx;
		if (mirror_y) koef_fy = -koef_fy;

		//distance filter
		pnt.resize(depth_w * depth_h);
		int k = 0;
		ofVec3f p;
		for (int y = 0; y < depth_h; y++) {
			for (int x = 0; x < depth_w; x++) {
				float z = rawDepthPixels[x + depth_w * y];
				if (z >= corner0.z && z <= corner1.z) {
					p.x = -(x - param.cx) * z * koef_fx;
					if (p.x >= corner0.x && p.x <= corner1.x) {
						p.y = -(y - param.cy) * z * koef_fy;
						if (p.y >= corner0.y && p.y <= corner1.y) {
							p.z = z;
							pnt[k++] = p;
						}
					}
				}
			}
		}
		pnt.resize(k);
	}
	return pnt;
}

//--------------------------------------------------------------------------------
