#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetWindowShape(1024, 600);
    ofBackground(60, 60, 60);
    ofTrueTypeFont::setGlobalDpi(72);
    
    futura68.loadFont("FuturaStd-Heavy.otf", 68);
    futura68.setLetterSpacing(1.1);
    futura68.setLineHeight(100);
    
    futura34.loadFont("FuturaStd-HeavyOblique.otf", 34);
    futura34.setLetterSpacing(1.05);
    futura34.setLineHeight(44);
    
    autor1.init("FuturaStd-HeavyOblique.otf", 34);
    autor1.setLineHeight(44);
    autor1.setText("Thomas J. Watson");
    autor1.setColor(200, 200, 200, 255);
    autor1.wrapTextX(ofGetWidth());
    
    autor2.init("FuturaStd-BookOblique.otf", 34);
    autor2.setLineHeight(44);
    autor2.setText("Vorsitzender von IBM, 1943");
    autor2.wrapTextX(ofGetWidth());
    autor2.setColor(200, 200, 200, 255);
    
    
    futura34.loadFont("FuturaStd-BookOblique.otf", 34);
    futura34.setLetterSpacing(1.05);
    futura34.setLineHeight(44);
    
    satz1afull = "ICH DENKE, ES GIBT\nWELTWEIT EINEN MARKT\nFUER VIELLEICHT FUENF\nCOMPUTER.";
    satz1bfull = "Thomas J. Watson";
    satz1cfull = "Vorsitzender von IBM, 1943";
    
    satz2afull ="2045 WIRD ARTIFIZIELLE\nINTELLIGENZ DIE\n MENSCHLICHE UEBERHOLEN.";
    satz2bfull ="Ray Kurzweil, Physiker und Autor, 1999";
    
    
    ofAddListener( zitateTimer.TIMER_STARTED , this, &ofApp::zitateTimerStartHandler ) ;
    ofAddListener( zitateTimer.TIMER_COMPLETE , this, &ofApp::zitateTimerStartHandler ) ;


    zitateTimer.setup(14000);
    clearTimer.setup(1000);
    zitateTimer.start(true);
    alpha = 0;
    
    //ofLog(OF_LOG_NOTICE, ofToString(autor1));
    
}

//--------------------------------------------------------------
void ofApp::update(){
    zitateTimer.update();
    

    // satz bilden
    text_position = ofClamp(text_position, 0, satz1afull.length() + satz1bfull.length() + satz1cfull.length());
    
    
    if ( last_text_pos != (int)text_position) {
        cite_index = CLAMP(text_position, 0, satz1afull.length());
        
        author_index = CLAMP(text_position - satz1afull.length(), 0, satz1bfull.length());
        
        author2_index = CLAMP(text_position - satz1afull.length() + satz1bfull.length(), 0, satz1cfull.length());
        
        
        satz1a = satz1afull.substr(0, cite_index);
        satz1b = satz1bfull.substr(0, author_index);
        satz1c = satz1cfull.substr(0, author2_index);
    }
    
    last_text_pos = text_position;
    timer += ofGetLastFrameTime() * 15;
    if (timer < 114) {
        text_position += ofGetLastFrameTime() * 15;
    }
    if (timer > 160) {
        text_position -= ofGetLastFrameTime() * 60;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(60, 60, 60);
    ofSetColor(225);
    
    zitateTimer.draw( 15 , 15 ) ;

    
    futura68.drawString(satz1a, 50, 120);
    alignment = OF_TEXT_ALIGN_RIGHT;
    //futura34.drawString(satz1b, 50, 530);
    
    if (text_position > 95) {
        alpha = CLAMP(alpha, 0, 255);
        //autor1.setColor(255, 255, 255, alpha);
        autor1.drawRight(955, 450);
        //autor2.setColor(255, 255, 255, alpha);
        autor2.drawRight(950, 500);
        
        //alpha = alpha + 1;
    }
    
    futura34.drawString(ofToString((int)text_position) + " - " + ofToString(cite_index) + " - " + ofToString(alpha), 30, 40);
    
}

void ofApp::zitateTimerStartHandler(int &args) {
    text_position = 0;
    timer, alpha = 0;
    last_text_pos = cite_index = author_index = author2_index = 0;
    zitateTimer.start(true);
}

void ofApp::zitateTimerCompleteHandler(int &args) {

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
