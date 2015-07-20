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
  if (!doReload) return;
  doReload = false;
  db.loadFile(dbPath);
  citationIDs.clear();
  buildCitationRun();
}

void ofApp::scheduleReload() {
  loadDB();
}

void ofApp::DumpCitationList(){
  ofLog(OF_LOG_NOTICE) << "---------------";
  ofLog(OF_LOG_NOTICE) << "Current citation ids:";
  stringstream ids;
  int count = 0;
  for(deque<int>::iterator i = citationIDs.begin(); i != citationIDs.end(); ++i) {
    ids << *i << "\t";
    if(++count%10 == 0) {
      ids << endl;
    }
  }
  ofLog(OF_LOG_NOTICE) << endl << ids.str();

  ofLog(OF_LOG_NOTICE) << "---------------";
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
  unsigned int limit = 0;
  for(deque<int>::iterator i = citationIDs.begin(); i != citationIDs.end(); ++i) {
    if(++limit == 10) break;
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
  broadCastClients("citing:"+ofToString(id));
}

void ofApp::scheduleUserComment() {
  scheduleDownload();
  
  ofLog() << "Scheduling latest user comment";
  
  deque<int>::iterator newestComment = citationIDs.end();
  
  for(deque<int>::iterator id = citationIDs.begin(); id != citationIDs.end(); ++id) {
    Citation* c = Citation::fromCSVRow(db, *id);
    if(c->reason == "comment" && *newestComment < c->id) {
      newestComment = id;
    }
    delete c;
  }
  
  if (newestComment != citationIDs.end()) {
    ofLog() << "Found latest user comment with id: " << *newestComment;
    int comment = *newestComment;
    citationIDs.erase(newestComment);
    citationIDs.push_front(comment);
  } else {
    ofLog() << "Could not find latest user comment";
  }
}
