#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//  ofSetLogLevel(OF_LOG_ERROR);
  
  ofSetVerticalSync(true);
  ofBackground(51, 99, 59);

  // db settings, non-settable
  dbPath = ofToDataPath("zitate.csv", true);
  
  // load settings
  
  watcher.registerAllEvents(this);
  fileFilter.addExtension("xml");
  fileFilter.addExtension("csv");

  std::string folderToWatch = ofToDataPath("", true);
  bool listExistingItemsOnStart = true;
  
  watcher.addPath(folderToWatch, listExistingItemsOnStart, &fileFilter);
  
  
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
  setupTimers();
  
  align = UL2_TEXT_ALIGN_INVALID;
  
  // kick off the process
  startTimers();
  
}

//--------------------------------------------------------------
void ofApp::update(){
  updateAllTimers();
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(51, 99, 59);
  ofSetColor(225);
  
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
  } else {
    if (bDebug) {
      ofBackground(255, 0, 0);
      mainFace.drawString("Debug: No citation found!", 50, 100);
    }
  }
  
  showTimersDebug();
  
}

