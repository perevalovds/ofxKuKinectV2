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
	pcloud.resize(kinects.size());
        
    //Note you don't have to use ofxKuKinectV2 as a shared pointer, but if you want to have it in a vector ( ie: for multuple ) it needs to be.
    for(int d = 0; d < kinects.size(); d++){
		KinectV2Settings settings;
		settings.rgb = false;	
        kinects[d] = shared_ptr <ofxKuKinectV2> (new ofxKuKinectV2());
        kinects[d]->open(deviceList[d].serial, settings);
    }


}

//--------------------------------------------------------------
void ofApp::update(){

    for(int d = 0; d < kinects.size(); d++){
        kinects[d]->update();
		if( kinects[d]->isFrameNew() ) {
            texDepth[d].loadData( kinects[d]->getDepthPixels() );
			pcloud[d] = kinects[d]->getPointCloud();			
        }
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	for (int d = 0; d < kinects.size(); d++) {
		ofTexture &tex = texDepth[d];
		float shiftY = 40 + ((10 + tex.getHeight() / 2) * d);
		ofSetColor(255);
		texDepth[d].draw(10, shiftY, tex.getWidth() / 2, tex.getHeight() / 2);
	}
	cam.begin();
	for (int d = 0; d < kinects.size(); d++) {
		ofMesh mesh;
		if (!pcloud[d].empty()) {
			mesh.addVertices((glm::vec3 *)&pcloud[d][0].x, pcloud[d].size());
		}
		ofPushMatrix();
		ofSetColor(255);
		if (d == 0) ofSetColor(255, 255, 0);
		if (d == 1) ofSetColor(255, 0, 255);
		if (d == 2) ofSetColor(0, 255, 255);
		if (d == 3) ofSetColor(0, 255, 0);
		ofScale(-1, 1, -1);
		ofScale(0.2, 0.2, 0.2);
		ofTranslate(0, 0, -2000);
		mesh.drawVertices();
		ofPopMatrix();
	}
	cam.end();
	string fps = "App FPS: " + ofToString(ofGetFrameRate(),0) + ",    Kinects FPS:";
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
