//
//  ofAppDB.cpp
//  CiteClient
//
//  Created by jens on 20.03.15.
//
//

#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::loadDB(){
  db.loadFile(dbPath);
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
