#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetWindowShape(1024, 600);
    ofBackground(60, 60, 60);
    ofTrueTypeFont::setGlobalDpi(72);
    
    futura60.loadFont("FuturaStd-Heavy.otf", 60);
    futura60.setLetterSpacing(1.1);
    
    futura24.loadFont("FuturaStd-Book.otf", 24);
    futura24.setLetterSpacing(1.05);
    
    satz1full = "ICH DENKE, ES GIBT \nWELTWEIT EINEN MARKT \nFUER VIELLEICHT FUENF \nCOMPUTER.";
    satz2full = "Thomas J. Watson, Vorsitzender von IBM, 1943";

    
    
    
    ofAddListener( zitateTimer.TIMER_STARTED , this, &ofApp::zitateTimerStartHandler ) ;

    zitateTimer.setup(10000);
    zitateTimer.start(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    zitateTimer.update();
    
    // satz bilden
    text_position = ofClamp(text_position, 0, satz1full.length() + satz2full.length());
    
    
    if ( last_text_pos != (int)text_position) {
        cite_index = CLAMP(text_position, 0, satz1full.length());
        
        author_index = CLAMP(text_position - satz1full.length(), 0, satz2full.length());
        
        
        satz1 = satz1full.substr(0, cite_index);
        satz2 = satz2full.substr(0, author_index);
    }
    
    last_text_pos = text_position;
    
    text_position += ofGetLastFrameTime() * 10;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(225);
    
    zitateTimer.draw( 15 , 15 ) ;
    
    futura60.drawString(satz1, 50, 120);
    futura24.drawString(satz2, 50, 530);
    
    futura24.drawString(ofToString((int)text_position) + " - " + ofToString(cite_index) + " - " + ofToString(author_index), 30, 40);
    
}

void ofApp::zitateTimerStartHandler(int &args) {
    text_position = 0;
    last_text_pos = cite_index = author_index = 0;
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
