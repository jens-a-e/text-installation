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
  citationIDs.clear();
  buildCitationRun();
}

void ofApp::scheduleReload() {
  loadDB();
}

void ofApp::DumpCitationList(){
  ofLog(OF_LOG_VERBOSE) << "---------------";
  ofLog(OF_LOG_VERBOSE) << "Current citation ids:";
  for (unsigned int i = 0; i< citationIDs.size(); i++) {
    ofLog(OF_LOG_VERBOSE) << citationIDs[i];
  }
  ofLog(OF_LOG_VERBOSE) << "---------------";
}

void ofApp::buildCitationRun(){
  // gather the ids
  if (db.numRows == 0) {
    ofLog(OF_LOG_WARNING, "No data in DB");
    return;
  }
  
  vector<int> ids;
  
  // substract header row from numRows!
  for (int r=0; r < db.numRows-1; r++) {
    ids.push_back(r);
  }
  
  if (currentCitation == NULL) {
    ofRandomize(ids);
  } else {
    int testId;
    do {
      ofRandomize(ids);
      testId = ids.at(0);
    } while(testId != currentCitation->id);
  }
  
  citationIDs.clear();
  for(vector<int>::iterator i = ids.begin(); i != ids.end(); i++) {
    citationIDs.push_back(*i);
  }
  DumpCitationList();
}

void ofApp::nextCitation(){
  
  if (citationIDs.empty()) {
    buildCitationRun();
  }
  
  if (citationIDs.empty()) {
    ofLog(OF_LOG_NOTICE) << "No ids to cite";
    return;
  }
  
  DumpCitationList();

  int next = citationIDs.front();
  citationIDs.pop_front();
  
  if(currentCitation != NULL) {
    delete currentCitation;
  }

  currentCitation = Citation::fromCSVRow(db, next);
  
  ofLog() << currentCitation->toString();
  
  notifyCiting(currentCitation->id);
  
  DumpCitationList();
}

bool ofApp::popCitation(int id){
  if (citationIDs.empty()) return true;
  bool found = false;
  for(deque<int>::iterator i = citationIDs.begin(); i != citationIDs.end(); ++i) {
    if (id == *i) {
      citationIDs.erase(i);
      found = true;
      break;
    }
  }
  return found;
}

void ofApp::notifyCiting(int id) {
  // update the
  // updateActiveCites(id);
  broadCastClients("citing:"+ofToString(id-1)); // -1!! because id start at 1
}

void ofApp::scheduleUserComment() {
  // find the comment with the highest id
  deque<int>::iterator newestComment = citationIDs.end();
  for(deque<int>::iterator id = citationIDs.begin(); id != citationIDs.end(); ++id) {
    Citation* c = Citation::fromCSVRow(db, *id);
    if(c->reason == "comment" && *newestComment < c->id) {
      newestComment = id;
    }
    delete c;
  }
  if (newestComment != citationIDs.end()) {
    int comment = *newestComment;
    citationIDs.erase(newestComment);
    citationIDs.push_front(comment);
  }
}
