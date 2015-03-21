#include "ofApp.h"

bool ofApp::bAlignByPixel = false;

//--------------------------------------------------------------
void ofApp::setup(){
//  ofSetLogLevel(OF_LOG_ERROR);
  
  Settings.load(ofToDataPath("settings.xml",true));
  
  bDebug = Settings.getValue("settings:debug", 0) > 0;
  
  ofSetFullscreen(Settings.getValue("settings:fullscreen", 1) > 0);
  
  if (bDebug) {
    ofSetLogLevel(OF_LOG_VERBOSE);
  } else {
    ofSetLogLevel(OF_LOG_ERROR);
  }
  
  ofSetVerticalSync(true);
  ofBackground(51, 99, 59);

  citeAmount = 0;
  
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
  
  mainFace.setLetterSpacing(Settings.getValue("settings:fonts:main:letter-spacing", 0.1));
  mainFace.setLineHeight(Settings.getValue("settings:fonts:main:line-height", 100));
  mainFace.setGlobalDpi(72);
  mainFace.setWordWrap(true);
  mainFace.setAlignByPixel(bAlignByPixel);
  mainFace.setUseLayoutCache(true);
  mainFace.loadFont(heavyFont, Settings.getValue("settings:fonts:main:size", 68), true, true);
  
  mainFaceLarge.setLetterSpacing(Settings.getValue("settings:fonts:main-large:letter-spacing", 0.1));
  mainFaceLarge.setLineHeight(Settings.getValue("settings:fonts:main-large:line-height", 100));
  mainFaceLarge.setGlobalDpi(72);
  mainFaceLarge.setWordWrap(true);
  mainFaceLarge.setAlignByPixel(bAlignByPixel);
  mainFaceLarge.setUseLayoutCache(true);
  mainFaceLarge.loadFont(heavyFont, Settings.getValue("settings:fonts:main-large:size", 68), true, true);
  
  citeNameFace.setLineHeight(44);
  citeNameFace.setWordWrap(false);
  citeNameFace.loadFont(heavyObliqueFont, 34);

  citeMetaFace.setLineHeight(44);
  citeMetaFace.setWordWrap(false);
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
      
      if(currentCitation->body.size() <= Settings.getValue("settings:use-large-from", -1)) {
        mainFaceLarge.drawString(cite_partial, 50, Settings.getValue("settings:fonts:main-large:line-height", 100), ofGetWidth()-100, 500, align);
//        mainFaceLarge.drawStringAsShapes(cite_partial, 50, Settings.getValue("settings:fonts:main-large:line-height", 100), ofGetWidth()-100, 500, align);
      } else {
        mainFace.drawString(cite_partial, 50, Settings.getValue("settings:fonts:main:line-height", 100), ofGetWidth()-100, 500, align);
//        mainFace.drawStringAsShapes(cite_partial, 50, Settings.getValue("settings:fonts:main:line-height", 100), ofGetWidth()-100, 500, align);
      }
      
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
  
  if (bDebug) {
    ofDrawBitmapString(ofToString(ofGetFrameRate(),2), ofGetWidth() - 50, 25);
    ofDrawBitmapString(bAlignByPixel ? "Aligned by pixel" : "Not aligned by pixel", ofGetWidth() - 200, 40);
  }
  
}

