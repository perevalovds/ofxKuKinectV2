#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //Uncomment for verbose info from libfreenect2
    //ofSetLogLevel(OF_LOG_VERBOSE);

    ofBackground(30, 30, 30);

    //see how many devices we have.
    ofxKuKinectV2 tmp;
    vector <ofxKuKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
    
    //allocate for this many devices
    kinects.resize(deviceList.size());
    texDepth.resize(kinects.size());
    texRGB.resize(kinects.size());
        
    //panel.setup("", "settings.xml", 10, 100);
    
    //Note you don't have to use ofxKinectV2 as a shared pointer, but if you want to have it in a vector ( ie: for multiple ) it needs to be.
    for(int d = 0; d < kinects.size(); d++){
        kinects[d] = shared_ptr <ofxKuKinectV2> (new ofxKuKinectV2());

		KinectV2Settings settings;
		settings.depth = true;
		settings.rgb = true;
		settings.register_depth_rgb = true;
		settings.pipe_gl = true;
        kinects[d]->open(deviceList[d].serial, settings);
    }


}

//--------------------------------------------------------------
void ofApp::update(){

    for(int d = 0; d < kinects.size(); d++){
        kinects[d]->update();
        if( kinects[d]->isFrameNew() ){
			float mindist = 500;
			float maxdist = 6000;
            texDepth[d].loadData( kinects[d]->getDepthPixels(mindist, maxdist) );
            texRGB[d].loadData( kinects[d]->getRgbPixels() );
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for(int d = 0; d < kinects.size(); d++){
        float dwHD = 1920/4;
        float dhHD = 1080/4;
        
        float shiftY = 50 + ((10 + texDepth[d].getHeight()) * d);
    
        texDepth[d].draw(200, shiftY);
        texRGB[d].draw(210 + texDepth[d].getWidth(), shiftY, dwHD, dhHD);
    }
	string fps = "App FPS: " + ofToString(ofGetFrameRate(), 0) + ",    Kinects FPS:";
	for (int d = 0; d < kinects.size(); d++) {
		fps += " " + ofToString(kinects[d]->fps(), 0);
	}
	ofDrawBitmapStringHighlight(fps, 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
