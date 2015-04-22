//
//  ofAppTimerStates.cpp
//  CiteClient
//
//  Created by jens on 20.03.15.
//
//

#include "ofApp.h"

void ofApp::setupTimers(){
  idle.setup(1500);
  ofAddListener( idle.TIMER_STARTED  , this, &ofApp::idleTimerStartHandler ) ;
  ofAddListener( idle.TIMER_COMPLETE , this, &ofApp::idleTimerCompleteHandler ) ;
  
  type.setup(40);
  ofAddListener( type.TIMER_STARTED  , this, &ofApp::typeTimerStartHandler ) ;
  ofAddListener( type.TIMER_COMPLETE , this, &ofApp::typeTimerCompleteHandler ) ;
  
  waitForMeta.setup(1000);
  ofAddListener( waitForMeta.TIMER_STARTED  , this, &ofApp::waitForMetaTimerStartHandler ) ;
  ofAddListener( waitForMeta.TIMER_COMPLETE , this, &ofApp::waitForMetaTimerCompleteHandler ) ;
  
  showMeta.setup(Settings.getValue("settings:showtime", 7000)); // this is the full cite show time! +1000 from before
  ofAddListener( showMeta.TIMER_STARTED  , this, &ofApp::showMetaTimerStartHandler ) ;
  ofAddListener( showMeta.TIMER_COMPLETE , this, &ofApp::showMetaTimerCompleteHandler ) ;
  
  waitRewind.setup(1000);
  ofAddListener( waitRewind.TIMER_STARTED  , this, &ofApp::waitRewindTimerStartHandler) ;
  ofAddListener( waitRewind.TIMER_COMPLETE , this, &ofApp::waitRewindTimerCompleteHandler ) ;
  
  rewind.setup(16);
  ofAddListener( rewind.TIMER_STARTED  , this, &ofApp::rewindTimerStartHandler ) ;
  ofAddListener( rewind.TIMER_COMPLETE , this, &ofApp::rewindTimerCompleteHandler ) ;
  
  timers.push_back(&idle);
  timers.push_back(&type);
  timers.push_back(&waitForMeta);
  timers.push_back(&showMeta);
  timers.push_back(&waitRewind);
  timers.push_back(&rewind);
}

void ofApp::startTimers(){
  int start = 42;
  idleTimerCompleteHandler(start);
}

void ofApp::updateAllTimers() {
  for(std::vector<ofxSimpleTimer*>::iterator timer = timers.begin(); timer != timers.end(); ++timer) {
    (*timer)->update();
  }
}

void ofApp::showTimersDebug() {
  if (bDebug) {
    int y = 10;
    for(std::vector<ofxSimpleTimer*>::iterator timer = timers.begin(); timer != timers.end(); ++timer) {
      (*timer)->draw( 15 , y );
      y += 15;
    }
  }
}

void ofApp::idleTimerStartHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Idle Timer Started!");
}

void ofApp::idleTimerCompleteHandler(int &args) {
  ofLog(OF_LOG_NOTICE,"Idle Timer Complete!");
  
  // set new randmozed bg color // shift by 10%, random
  float h,s,b;
  h = bgHue + bgHue*ofRandom(-0.1, 0.1);
  s = bgSat + bgSat*ofRandom(-0.1, 0.1);
  b = bgBright + bgBright*ofRandom(-0.1, 0.1);
  
  bgColor.setHsb(h,s,b);
  
  cite_partial = "";
  nextCitation();
  citeAmount = 0;
  type.start(false);
}

void ofApp::typeTimerStartHandler(int &args) {
  //  ofLog(OF_LOG_NOTICE,"Type Timer Started!");
  
  if (currentCitation == NULL) {
    ofLog() << "No citation found, trying again later...";
    type.stop();
    idle.start(false); // try again later...
  }
  
  citeAmount++;
  
  cite_partial = currentCitation->body.substr(0,citeAmount);
}

void ofApp::typeTimerCompleteHandler(int &args) {
  //  ofLog(OF_LOG_NOTICE,"Type Timer Complete!");
  if (citeAmount >= currentCitation->body.length()) {
    // Citation is fully displayed
    waitForMeta.start(false);
  } else {
    type.start(false); // otherwise type next char!
  }
}

void ofApp::waitForMetaTimerStartHandler(int &args) {
  //  ofLog(OF_LOG_NOTICE,"WaitForMeta Timer Started!");
}

void ofApp::waitForMetaTimerCompleteHandler(int &args) {
  //  ofLog(OF_LOG_NOTICE,"WaitForMeta Complete!");
  showMeta.start(false);
}

void ofApp::showMetaTimerStartHandler(int &args) {
  //  ofLog(OF_LOG_NOTICE,"WaitForMeta Timer Started!");
}

void ofApp::showMetaTimerCompleteHandler(int &args) {
  //  ofLog(OF_LOG_NOTICE,"ShowMeta Complete!");
  waitRewind.start(false);
}

void ofApp::waitRewindTimerStartHandler(int &args) {
  //  ofLog(OF_LOG_NOTICE,"ShowMeta Timer Started!");
}

void ofApp::waitRewindTimerCompleteHandler(int &args) {
  //  ofLog(OF_LOG_NOTICE,"WaitRewind Complete!");
  rewind.start(false);
}

void ofApp::rewindTimerStartHandler(int &args) {
  //  ofLog(OF_LOG_NOTICE,"Rewind Timer Started!");
  citeAmount--;
  cite_partial = currentCitation->body.substr(0,citeAmount);
}

void ofApp::rewindTimerCompleteHandler(int &args) {
  //  ofLog(OF_LOG_NOTICE,"Rewind Complete!");
  if (citeAmount > 0) {
    rewind.start(false);
  } else {
    idle.start(false);
  }
}

