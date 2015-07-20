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
  ofLog(OF_LOG_VERBOSE) << "---------------" << endl;
  ofLog(OF_LOG_VERBOSE) << "Current citation ids:" << endl;
  for (deque<int>::iterator id = citationIDs.begin(); id != citationIDs.end(); ++id) {
    ofLog(OF_LOG_VERBOSE) << *id;
  }
  ofLog(OF_LOG_VERBOSE) << "---------------" << endl;
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
  for(vector<int>::iterator i = ids.begin(); i != ids.end(); ++i) {
    citationIDs.push_back(*i);
  }
  DumpCitationList();
}

//const bool useServer = false;
void ofApp::nextCitation(){
//  if (useServer) {
//    
//    // try server (this is not working properly on the server side!!)
//    ofHttpResponse nextCite = ofLoadURL("http://master.text:4200/cgi-bin/test.lua");
//    if(  nextCite.status != 200 || nextCite.data.size() == 0) {
//      ofLog(OF_LOG_ERROR, "Incorrect response from server, using fallback citing.");
//      goto local;
//    }
//    
//    ofLog(OF_LOG_ERROR, nextCite.data.getText());
//    currentCitation = Citation::fromString(nextCite.data.getText());
//    if (currentCitation == NULL) {
//      ofLog(OF_LOG_ERROR, "Could not parse as citation: \n"+nextCite.data.getText());
//      goto local;
//    }
//    return;
//  }
//  
//  // fallback to local random
//local:
//  if (doReload) {
//    db.loadFile(dbPath);
//    citationIDs.clear();
//    doReload = false;
//  }
  
  if (citationIDs.empty()) {
    buildCitationRun();
  }
  
  DumpCitationList();
  
  if (citationIDs.empty()) return;
  
  int next = citationIDs.front();
  citationIDs.pop_front();
  
  if(currentCitation != NULL) {
    delete currentCitation;
  }

  currentCitation = Citation::fromCSVRow(db, next);
  
  ofLog() << currentCitation->toString();
  notifyCiting(currentCitation->id);
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
  broadCastClients("citing:"+ofToString(id));
}

void ofApp::updateActiveCites(int id) {
  if (activeCites.size() >= 10) activeCites.clear();
  activeCites.insert(id);
}

bool ofApp::inActiveCites(int id){
  return activeCites.find(id) != activeCites.end();
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
    citationIDs.push_front(*newestComment);
    citationIDs.erase(newestComment);
  }
}
