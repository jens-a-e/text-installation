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
  
  ofSetFrameRate(60);
  ofSetVerticalSync(true);
  ofBackground(51, 99, 59);

  citeAmount = 0;
  bitmapRendering = true;
  
  // db settings, non-settable
  dbPath = ofToDataPath("zitate.csv", true);
  
  // load settings
  
  watcher.registerAllEvents(this);
  fileFilter.addExtension("xml");
  fileFilter.addExtension("csv");

  std::string folderToWatch = ofToDataPath("", true);
  bool listExistingItemsOnStart = true;
  
  watcher.addPath(folderToWatch, listExistingItemsOnStart, &fileFilter);
  
  // Prepare fonts
  loadFonts();
  
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

  if (currentCitation != NULL) {
    
    ofSetColor(225);
    
    
    if (type.bIsRunning || waitForMeta.bIsRunning || showMeta.bIsRunning || waitRewind.bIsRunning || rewind.bIsRunning) {

      if (currentCitation->affiliation == "comment") {
        ofBackground(33, 102, 137);
      }
      
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
      
      if (currentCitation->affiliation.length() > 0 && currentCitation->affiliation != "comment") {
        meta += currentCitation->affiliation;
      }
      
      if (currentCitation->year.length() > 0) {
        meta += (meta.length() == 0 ? "" : ", ") + currentCitation->year;
      }
      
      if (currentCitation->reason.length() > 0) {
        meta += (meta.length() == 0 ? "" : "\n") + currentCitation->reason;
      }
      
      citeMetaFace.drawString(meta, 50, ofGetHeight()- 4*34 + 5 , ofGetWidth()-105, 3*34, UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_RIGHT);
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

