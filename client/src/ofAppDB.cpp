//
//  ofAppDB.cpp
//  CiteClient
//
//  Created by jens on 20.03.15.
//
//

#include "ofApp.h"

static bool doReload = false;
//--------------------------------------------------------------
void ofApp::loadDB(){
  doReload = false;
  db.loadFile(dbPath);
  buildCitationRun();
}

void ofApp::scheduleReload() {
  doReload = true;
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
  
  ofRandomize(ids);
  
  string last = currentCitation == NULL ? "" : currentCitation->body;
  
  if (last != "") {
    Citation *testCite;
    do {
      ofRandomize(ids);
      testCite = Citation::fromCSVRow(db, ids.at(0));
    } while(testCite->body != last );
    delete testCite;
  }
  
  if (citationIDs != NULL) {
    delete citationIDs;
  }
  
  citationIDs = new std::stack<int,std::vector<int> >(ids);
}

void ofApp::nextCitation(){
  if (doReload) {
    db.loadFile(dbPath);
    doReload = false;
  }
  
  if (citationIDs == NULL || citationIDs->size() == 0) {
    buildCitationRun();
  }
  
  int next = currentCitationID();
  
  if (next < 0) return;
  
  if(currentCitation != NULL) {
    delete currentCitation;
  }
  
  if (mustBeComment) {
    mustBeComment = false;
    currentCitation = Citation::fromCSVRow(db, db.numRows-2);
  } else {
    currentCitation = Citation::fromCSVRow(db, next);
    citationIDs->pop();
  }
  
  ofLog() << currentCitation->toString();
  broadCastClients("Citing:"+ofToString(currentCitation->id));
  
}

void ofApp::scheduleUserComment() {
  mustBeComment = true;
}

int ofApp::currentCitationID() {
  return citationIDs == NULL ? -1 : citationIDs->top();
}
