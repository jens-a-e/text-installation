#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetLogLevel(OF_LOG_ERROR);
  
  ofSetVerticalSync(true);
  ofBackground(51, 99, 59);

  string bookObliqueFont  = ofToDataPath("FuturaStd-BookOblique.otf");
  string heavyFont        = ofToDataPath("FuturaStd-Heavy.otf");
  string heavyObliqueFont = ofToDataPath("FuturaStd-HeavyOblique.otf");

  mainFace.setLetterSpacing(0.1);
  mainFace.setLineHeight(100);
  mainFace.setSpaceSize(1);
  mainFace.setGlobalDpi(72);
  mainFace.setAlignByPixel(true);
  mainFace.setWordWrap(true);
  mainFace.useProportional(true);
  mainFace.setTextDirection(UL2_TEXT_DIRECTION_LTR);
  mainFace.loadFont(heavyFont, 68);
  
  citeNameFace.setLineHeight(44);
  citeNameFace.setWordWrap(true);
  citeNameFace.loadFont(heavyObliqueFont, 34);

  citeMetaFace.setLineHeight(44);
  citeMetaFace.setWordWrap(true);
  citeMetaFace.loadFont(bookObliqueFont, 34);
  
  // Prepare citations
  
  loadDB();  
  
  // -- Setup Timers
  idle.setup(1500);
  ofAddListener( idle.TIMER_STARTED  , this, &ofApp::idleTimerStartHandler ) ;
  ofAddListener( idle.TIMER_COMPLETE , this, &ofApp::idleTimerCompleteHandler ) ;

#ifndef CPM
#define CPM(x) (60/float(x)*1000)
#endif

#define FCPM CPM(800)
#define RCPM CPM(1000)

  ofLog() << "Using " << ofToString(FCPM) << " as forward typing frequency";
  type.setup(40);
  ofAddListener( type.TIMER_STARTED  , this, &ofApp::typeTimerStartHandler ) ;
  ofAddListener( type.TIMER_COMPLETE , this, &ofApp::typeTimerCompleteHandler ) ;

  waitForMeta.setup(1000);
  ofAddListener( waitForMeta.TIMER_STARTED  , this, &ofApp::waitForMetaTimerStartHandler ) ;
  ofAddListener( waitForMeta.TIMER_COMPLETE , this, &ofApp::waitForMetaTimerCompleteHandler ) ;
  
  showMeta.setup(3000);
  ofAddListener( showMeta.TIMER_STARTED  , this, &ofApp::showMetaTimerStartHandler ) ;
  ofAddListener( showMeta.TIMER_COMPLETE , this, &ofApp::showMetaTimerCompleteHandler ) ;
  
  waitRewind.setup(1000);
  ofAddListener( waitRewind.TIMER_STARTED  , this, &ofApp::waitRewindTimerStartHandler) ;
  ofAddListener( waitRewind.TIMER_COMPLETE , this, &ofApp::waitRewindTimerCompleteHandler ) ;
  
  ofLog() << "Using " << ofToString(RCPM) << " as rewidn typing frequency";
  rewind.setup(16);
  ofAddListener( rewind.TIMER_STARTED  , this, &ofApp::rewindTimerStartHandler ) ;
  ofAddListener( rewind.TIMER_COMPLETE , this, &ofApp::rewindTimerCompleteHandler ) ;
  
  timers.push_back(&idle);
  timers.push_back(&type);
  timers.push_back(&waitForMeta);
  timers.push_back(&showMeta);
  timers.push_back(&waitRewind);
  timers.push_back(&rewind);
  
  align = UL2_TEXT_ALIGN_INVALID;
  
  // kick off the process
  int start = 42;
  idleTimerCompleteHandler(start);
  
}

//--------------------------------------------------------------
void ofApp::update(){
  
  for(std::vector<ofxSimpleTimer*>::iterator timer = timers.begin(); timer != timers.end(); ++timer) {
    (*timer)->update();
  }

}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(51, 99, 59);
  ofSetColor(225);
  
  if (bDebug) {
    int y = 10;
    for(std::vector<ofxSimpleTimer*>::iterator timer = timers.begin(); timer != timers.end(); ++timer) {
      (*timer)->draw( 15 , y );
      y += 15;
    }
  }
  

  if (currentCitation != NULL) {
    if (type.bIsRunning || waitForMeta.bIsRunning || showMeta.bIsRunning || waitRewind.bIsRunning || rewind.bIsRunning) {
      mainFace.drawString(cite_partial, 50, 100, ofGetWidth()-100, 500, align);
    }
    
    if (showMeta.bIsRunning) {
      
      citeNameFace.drawString(currentCitation->author ,50, ofGetHeight() - 4*34 - 50, ofGetWidth()-100, 4*34, UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_RIGHT);
      
      // affilition[,  year]
      // reason
      string meta = "";
      
      if (currentCitation->affiliation.length() > 0) {
        meta += currentCitation->affiliation;
      }
      
      if (currentCitation->year.length() > 0) {
        meta += (meta.length() == 0 ? "" : ", ") + currentCitation->year;
      }
      
      if (currentCitation->reason.length() > 0) {
        meta += (meta.length() == 0 ? "" : "\n") + currentCitation->reason;
      }
      
      citeMetaFace.drawString(meta, 50, ofGetHeight()- 4*34 , ofGetWidth()-100, 3*34, UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_RIGHT);
    }
  }
  
}

void ofApp::idleTimerStartHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Idle Timer Started!");
}

static int citeAmount = 0;

void ofApp::idleTimerCompleteHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Idle Timer Complete!");
  cite_partial = "";
  nextCitation();
  citeAmount = 0;
  type.start(false);
}

void ofApp::typeTimerStartHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Type Timer Started!");
  
  if (currentCitation == NULL) {
    ofLog() << "No citation found, trying again later...";
    type.stop();
    idle.start(false); // try again later...
  }
  
  citeAmount++;
  
  cite_partial = currentCitation->body.substr(0,citeAmount);
}

void ofApp::typeTimerCompleteHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Type Timer Complete!");
  if (citeAmount >= currentCitation->body.length()) {
    // Citation is fully displayed
    waitForMeta.start(false);
  } else {
    type.start(false); // otherwise type next char!
  }
}

void ofApp::waitForMetaTimerStartHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"WaitForMeta Timer Started!");
}

void ofApp::waitForMetaTimerCompleteHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"WaitForMeta Complete!");
  showMeta.start(false);
}

void ofApp::showMetaTimerStartHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"WaitForMeta Timer Started!");
}

void ofApp::showMetaTimerCompleteHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"ShowMeta Complete!");
  waitRewind.start(false);
}

void ofApp::waitRewindTimerStartHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"ShowMeta Timer Started!");
}

void ofApp::waitRewindTimerCompleteHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"WaitRewind Complete!");
  rewind.start(false);
}

void ofApp::rewindTimerStartHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Rewind Timer Started!");
  citeAmount--;
  cite_partial = currentCitation->body.substr(0,citeAmount);
}

void ofApp::rewindTimerCompleteHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Rewind Complete!");
  if (citeAmount > 0) {
    rewind.start(false);
  } else {
    idle.start(false);
  }
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
      case '.':
      bDebug = !bDebug;
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
  // substract header row from numRows!
  for (int r=0; r < db.numRows-1; r++) {
    ids.push_back(r);
  }
  
  int last_id = currentCitation == NULL ? -1 : currentCitation->id;
  
  if (last_id >= 0) {
    Citation *testCite;
    do {
      ofRandomize(ids);
      testCite = Citation::fromCSVRow(db, ids.at(0));
    } while(testCite->id != last_id );
    delete testCite;
  }
  
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
  
  currentCitation = Citation::fromCSVRow(db, next);
  ofLog() << currentCitation->toString();
  
  citationIDs->pop();
}

int ofApp::currentCitationID() {
  return citationIDs == NULL ? -1 : citationIDs->top();
}
