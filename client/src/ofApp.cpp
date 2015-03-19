#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
  ofSetVerticalSync(true);
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
  
  // Prepare citations
  
  loadDB();  

  satz1afull = L"ICH DENKE, ES GIBT\nWELTWEIT EINEN MARKT\nFUER VIELLEICHT FUENF\nCOMPUTER.";
  satz1bfull = L"Thomas J. Watson";
  satz1cfull = L"Vorsitzender von IBM, 1943";
  
  
  // -- Setup Timers
  
  ofAddListener( zitateTimer.TIMER_STARTED  , this, &ofApp::zitateTimerStartHandler ) ;
  ofAddListener( zitateTimer.TIMER_COMPLETE , this, &ofApp::zitateTimerCompleteHandler ) ;

  zitateTimer.setup(8000);
  
  ofAddListener( rewindTimer.TIMER_STARTED  , this, &ofApp::rewindTimerStartHandler ) ;
  ofAddListener( rewindTimer.TIMER_COMPLETE , this, &ofApp::rewindTimerCompleteHandler ) ;
  
  rewindTimer.setup(900);
  
  ofAddListener( clearTimer.TIMER_STARTED  , this, &ofApp::clearTimerStartHandler ) ;
  ofAddListener( clearTimer.TIMER_COMPLETE , this, &ofApp::clearTimerCompleteHandler ) ;

  clearTimer.setup(1500);
  
  zitateTimer.start(false);
  
  align = UL2_TEXT_ALIGN_INVALID;

  // -- Helper Vars
  alpha = 0;
  
}

//--------------------------------------------------------------
void ofApp::update(){
  zitateTimer.update();
  rewindTimer.update();
  clearTimer.update();

  if (currentCitation == NULL) {
    return;
  }
  
  int maxChars = currentCitation->body.length();
  float amount = 0;
  if (zitateTimer.bIsRunning) {
    float currentTimer = zitateTimer.getNormalizedProgress();
    amount = ofLerp(0, maxChars, ofMap(currentTimer, 0, 0.6, 0, 1));
  } else if (rewindTimer.bIsRunning) {
    float currentTimer = rewindTimer.getNormalizedProgress();
    amount = ofLerp(0, maxChars, ofMap(currentTimer, 0, 1, 1, 0));
  }
  
  satz1a = currentCitation->body.substr(0,(int)ofClamp(amount, 0, maxChars));
  
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(51, 99, 59);
  ofSetColor(225);
  
  zitateTimer.draw( 15 , 10 );
  clearTimer.draw( 15 , 25 );
  rewindTimer.draw(15, 40 );

  if (currentCitation != NULL) {
    if (satz1a.length() > 0) {
      mainFace.drawString(satz1a, 50, 100, ofGetWidth()-100, 500, align);
    }
    
    if (zitateTimer.getNormalizedProgress() >= 0.75 && zitateTimer.getNormalizedProgress() <= 0.95) {
      citeNameFace.drawString(currentCitation->author ,50, 550, ofGetWidth()-100, 50, UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_RIGHT);
      citeMetaFace.drawString(currentCitation->affiliation,50, 600, ofGetWidth()-100, 50, UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_RIGHT);
    }
  }
  
}

void ofApp::zitateTimerStartHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Zitate Timer Started!");
  nextCitation();
}

void ofApp::zitateTimerCompleteHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Zitate Timer Complete!");
  rewindTimer.start(false);
}

void ofApp::clearTimerStartHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Clear Timer Started!");
}

void ofApp::clearTimerCompleteHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Clear Timer Complete!");
  zitateTimer.start(false);
}

void ofApp::rewindTimerStartHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Rewind Timer Started!");
}

void ofApp::rewindTimerCompleteHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Rewind Timer Complete!");
  clearTimer.start(false);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  string strDirection = "";
  string strAlign = "";
  switch(key){
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

//--------------------------------------------------------------
void ofApp::loadDB(){
  db.loadFile(ofToDataPath("zitate.csv"));
  buildCitationRun();
}

void ofApp::buildCitationRun(){
  // gather the ids
  if (db.numRows == 0) {
    ofLog(OF_LOG_WARNING, "No data in DB");
    return;
  }
  std:vector<int> ids;
  for (int r=0; r < db.numRows; r++) {
    ids.push_back(r);
  }
  ofRandomize(ids);
  if (citationIDs != NULL) {
    delete citationIDs;
  }
  citationIDs = new std::stack<int,std::vector<int> >(ids);
}

void ofApp::nextCitation(){
  if (citationIDs == NULL || citationIDs->size() == 0) {
    buildCitationRun();
  }
  
  int next = currentCitationID();
  if (next < 0) return;
  
  if(currentCitation != NULL) {
    delete currentCitation;
  }
  currentCitation = new Citation(Citation::fromCSVRow(db, next));
  citationIDs->pop();
}

int ofApp::currentCitationID() {
  return citationIDs == NULL ? -1 : citationIDs->top();
}
