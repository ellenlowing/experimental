#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//    ofBackground(0);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
    
	font.load("type/verdana.ttf", 100, true, false, true, 0.4, 72);
	#ifdef TARGET_OPENGLES
	shader.load("shaders_gles/noise.vert","shaders_gles/noise.frag");
    printf("shader_gles\n");
	#else
	if(ofIsGLProgrammableRenderer()){
		shader.load("shaders_gl3/noise.vert", "shaders_gl3/noise.frag");
        printf("shader_gl3\n");
	}else{
		shader.load("shaders/noise.vert", "shaders/noise.frag");
        printf("shader\n");
	}
	#endif

	doShader = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i = 0; i < 7; i++) {
        if(toggle[i]) {
            if(trigger[i] <= 1.0) trigger[i] += 0.05;
        } else {
            if(trigger[i] >= 0.0) trigger[i] -= 0.05;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    shader.begin();
    shader.setUniform2f("u_mouse", (float)mouseX/ofGetWidth(), (float)mouseY/ofGetHeight() );
    shader.setUniform1f("u_time", ofGetElapsedTimef());
    shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1);
    shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18);
    shader.setUniform1f("u_trigger1", trigger[0]);
    shader.setUniform1f("u_trigger2", trigger[1]);
    shader.setUniform1f("u_trigger3", trigger[2]);
    shader.setUniform1f("u_trigger4", trigger[3]);
    shader.setUniform1f("u_trigger5", trigger[4]);
    shader.setUniform1f("u_trigger6", trigger[5]);
    shader.setUniform1f("u_trigger7", trigger[6]);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
	
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 

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
    if(x <= ofGetWidth()/7) {
        toggle[0] = 1 - toggle[0];
        printf("%d\n", 1);
    }
    
    if (x > ofGetWidth()/7 && x <= 2*ofGetWidth()/7) {
        toggle[1] = 1 - toggle[1];
        printf("%d\n", 2);
    }
    
    if (x > 2*ofGetWidth()/7 && x <= 3*ofGetWidth()/7) {
        toggle[2] = 1 - toggle[2];
        printf("%d\n", 3);
    }
    
    if (x > 3*ofGetWidth()/7 && x <= 4*ofGetWidth()/7) {
        toggle[3] = 1 - toggle[3];
        printf("%d\n", 4);
    }
    
    if (x > 4*ofGetWidth()/7 && x <= 5*ofGetWidth()/7) {
        toggle[4] = 1 - toggle[4];
        printf("%d\n", 5);
    }
    
    if (x > 5*ofGetWidth()/7 && x <= 6*ofGetWidth()/7) {
        toggle[5] = 1 - toggle[5];
        printf("%d\n", 6);
    }
    
    if (x > 6*ofGetWidth()/7 && x <= ofGetWidth()) {
        toggle[6] = 1 - toggle[6];
        printf("%d\n", 7);
    }
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
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

