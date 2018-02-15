//
//  ofxKuKinectV2_ext.cpp
//
//  Using depth data without Kinect device.
//	This module can be compiled and used alone, without the whole addon
//

#include "ofxKuKinectV2_ext.h"

void ofxKuKinectV2_get_point_cloud(vector<ofPoint> &points_out, unsigned short *depth, int w, int h,
	float kinect_cx, float kinect_cy, float kinect_fx, float kinect_fy) {
	points_out.resize(w * h);
	int k = 0;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			float z = depth[k];
			ofVec3f &p = points_out[k];
			k++;
			p.z = z;
			p.x = (x - kinect_cx) * z / kinect_fx;
			p.y = -(y - kinect_cy) * z / kinect_fy;
		}
	}
}

//--------------------------------------------------------------------------------
void ofxKuKinectV2_get_pixels(ofPixels &pixels_out, unsigned short *depth, int w, int h, float mindist, float maxdist) {
	pixels_out.allocate(w, h, 1);
	unsigned char *pixels = pixels_out.getData();
	for (int i = 0; i < w*h; i++) {
		pixels[i] = ofMap(depth[i], mindist, maxdist, 255, 0, true);
		if (pixels[i] == 255) {
			pixels[i] = 0;
		}		
	}
}

//--------------------------------------------------------------------------------
//declare pixels here to eliminate memory allocation/deallocation, and so optimize performance
ofPixels ofxKuKinectV2_get_texture_pixels__;
void ofxKuKinectV2_get_texture(ofTexture &texture_out, unsigned short *depth, int w, int h, float mindist, float maxdist) {
	ofxKuKinectV2_get_pixels(ofxKuKinectV2_get_texture_pixels__, depth, w, h, mindist, maxdist);
	texture_out.loadData(ofxKuKinectV2_get_texture_pixels__);
}

//--------------------------------------------------------------------------------

