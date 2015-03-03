#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
  ofSetVerticalSync(true);
  ofSetWindowShape(1024, 600);
  ofBackground(51, 99, 59);

  ofTrueTypeFont::setGlobalDpi(72);

  string bookObliqueFont  = ofToDataPath("FuturaStd-BookOblique.otf");
  string heavyFont        = ofToDataPath("FuturaStd-Heavy.otf");
  string heavyObliqueFont = ofToDataPath("FuturaStd-HeavyOblique.otf");

  mainFace.setLetterSpacing(0.1);
  mainFace.setLineHeight(100);
  mainFace.setSpaceSize(1);
  mainFace.setGlobalDpi(72);
  mainFace.setAlignByPixel(true);
  mainFace.setWordWrap(false);
  mainFace.useProportional(true);
  mainFace.setTextDirection(UL2_TEXT_DIRECTION_LTR);
  mainFace.loadFont(heavyFont, 68);

  utilFace.loadFont(heavyFont, 34);
  utilFace.setLetterSpacing(1.05);
  utilFace.setLineHeight(44);
  utilFace.setGlobalDpi(72);
  utilFace.setAlignByPixel(true);
  utilFace.setWordWrap(true);
  utilFace.setTextDirection(UL2_TEXT_DIRECTION_TTB);
  
  citeNameFace.loadFont(heavyObliqueFont, 34);
  citeNameFace.setLineHeight(44);
  citeNameFace.setWordWrap(true);

  citeMetaFace.loadFont(bookObliqueFont, 34);
  citeMetaFace.setLineHeight(44);
  citeMetaFace.setWordWrap(true);

  satz1afull = L"ICH DENKE, ES GIBT\nWELTWEIT EINEN MARKT\nFUER VIELLEICHT FUENF\nCOMPUTER.";
  satz1bfull = L"Thomas J. Watson";
  satz1cfull = L"Vorsitzender von IBM, 1943";
  
  
  // -- Setup Timers
  
  ofAddListener( zitateTimer.TIMER_STARTED  , this, &ofApp::zitateTimerStartHandler ) ;
  ofAddListener( zitateTimer.TIMER_COMPLETE , this, &ofApp::zitateTimerCompleteHandler ) ;

  zitateTimer.setup(14000);
  clearTimer.setup(1000);
  zitateTimer.start(true);

  // -- Helper Vars
  alpha = 0;
  
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
  
  if (text_position < 0) text_position = 0.5;
  
    timer += ofGetLastFrameTime() * 15;
    if (timer < 114) {
      text_position += ofGetLastFrameTime() * 15;
    }
    else if (timer > 160 && text_position > 0) {
      text_position -= ofGetLastFrameTime() * 60;
    } else if (text_position < 0) {
      zitateTimer.reset();
      zitateTimer.start(true);
    }
  
  ofLog(OF_LOG_NOTICE, "textposition %f\t%f\n", text_position,ofGetLastFrameTime());
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(60, 60, 60);
  ofSetColor(225);
  
  zitateTimer.draw( 15 , 15 ) ;

  mainFace.drawString(satz1a, 50, 120, 1300, 500, align);

  if (text_position > 95) {
    citeNameFace.drawString(satz1bfull,955, 450, 1300, 50, align);
    citeMetaFace.drawString(satz1cfull,950, 500, 1300, 50, align);
  }
  
  utilFace.drawString(ofToString((int)text_position) + " - " + ofToString(cite_index) + " - " + ofToString(alpha), 30, 40);
  
}

void ofApp::zitateTimerStartHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Zitate Timer Complete!");
    text_position = 0;
    timer, alpha = 0;
    last_text_pos = cite_index = author_index = author2_index = 0;
    zitateTimer.start(true);
}

void ofApp::zitateTimerCompleteHandler(int &args) {
  text_position = 0;
  timer, alpha = 0;
  last_text_pos = cite_index = author_index = author2_index = 0;
  ofLog(OF_LOG_NOTICE,"Zitate Timer Complete!");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  string strDirection = "";
  string strAlign = "";
  switch(key){
    case ' ':
      bitmapRendering=!bitmapRendering;
      break;
    case 'w':
    case 'W':
      mainFace.setWordWrap(!mainFace.getWordWrap());
      break;
    case 'q':
    case 'Q':
      mainFace.setUseLayoutCache(!mainFace.getUseLayoutCache());
      break;
    case 'e':
    case 'E':
      mainFace.setAlignByPixel(!mainFace.getAlignByPixel());
      break;
    case '0':
      align = UL2_TEXT_ALIGN_INVALID;
      strAlign="No Alignment [0]";
      break;
    case '1':
      align = UL2_TEXT_ALIGN_V_BOTTOM|UL2_TEXT_ALIGN_LEFT;
      strAlign="bottom-left [1]";
      break;
    case '2':
      align = UL2_TEXT_ALIGN_V_BOTTOM|UL2_TEXT_ALIGN_CENTER;
      strAlign="bottom-center [2]";
      break;
    case '3':
      align = UL2_TEXT_ALIGN_V_BOTTOM|UL2_TEXT_ALIGN_RIGHT;
      strAlign="bottom-right [3]";
      break;
    case '4':
      align = UL2_TEXT_ALIGN_V_MIDDLE|UL2_TEXT_ALIGN_LEFT;
      strAlign="middle-left [4]";
      break;
    case '5':
      align = UL2_TEXT_ALIGN_V_MIDDLE|UL2_TEXT_ALIGN_CENTER;
      strAlign="middle-center [5]";
      break;
    case '6':
      align = UL2_TEXT_ALIGN_V_MIDDLE|UL2_TEXT_ALIGN_RIGHT;
      strAlign="middle-right [6]";
      break;
    case '7':
      align = UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_LEFT;
      strAlign="top-left [7]";
      break;
    case '8':
      align = UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_CENTER;
      strAlign="top-center [8]";
      break;
    case '9':
      align = UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_RIGHT;
      strAlign="top-right [9]";
      break;
    case 'a':
    case 'A':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_LTR,UL2_TEXT_DIRECTION_TTB);
      strDirection="Left to Right (TTB)";
      break;
    case 's':
    case 'S':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_RTL,UL2_TEXT_DIRECTION_TTB);
      strDirection="Right to Left (TTB)";
      break;
    case 'd':
    case 'D':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_TTB,UL2_TEXT_DIRECTION_LTR);
      strDirection="Top to Bottom (LTR)";
      break;
    case 'f':
    case 'F':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_BTT,UL2_TEXT_DIRECTION_LTR);
      strDirection="Bottom to Top (LTR)";
      break;
    case 'z':
    case 'Z':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_LTR,UL2_TEXT_DIRECTION_BTT);
      strDirection="Left to Right (BTT)";
      break;
    case 'x':
    case 'X':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_RTL,UL2_TEXT_DIRECTION_BTT);
      strDirection="Right to Left (BTT)";
      break;
    case 'c':
    case 'C':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_TTB,UL2_TEXT_DIRECTION_RTL);
      strDirection="Top to Bottom (RTL)";
      break;
    case 'v':
    case 'V':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_BTT,UL2_TEXT_DIRECTION_RTL);
      strDirection="Bottom to Top (RTL)";
      break;
      
  }
  
  ofLog(OF_LOG_NOTICE, strAlign);
  ofLog(OF_LOG_NOTICE, strDirection);
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
