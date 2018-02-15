//
//  ofxKuKinectV2_ext.cpp
//
//  Using depth data without Kinect device.
//	This module can be compiled and used alone, without the whole addon
//


#pragma once

#include "ofMain.h"


void ofxKuKinectV2_get_point_cloud(vector<ofPoint> &points_out, unsigned short *depth, int w, int h, float kinect_cx, float kinect_cy, float kinect_fx, float kinect_fy);

void ofxKuKinectV2_get_pixels(ofPixels &pixels_out, unsigned short *depth, int w, int h, float mindist = 500, float maxdist = 6000);

void ofxKuKinectV2_get_texture(ofTexture &texture_out, unsigned short *depth, int w, int h, float mindist = 500, float maxdist = 6000);
