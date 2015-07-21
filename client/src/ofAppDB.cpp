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
  comments.loadFile(commentsPath);
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
  
  vector<int> ids;
  
  // substract header row from numRows!
  for (int r=1; r <= db.numRows; r++) {
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
//  ofExit();
}

void ofApp::nextCitation(){
  
  if(doDownload) {
    bool newCites = downloadCites();
    if (newCites) {
      ofLog() << "Downloaded new citations.";
    }
    doDownload = true;
    bool newComments = downloadComments();
    if (newComments) {
      ofLog() << "Downloaded new comments.";
    }
    doDownload = !newCites || !newComments; // check wether to retry next time!
    doReload = newCites || newComments;
  }
  
  if (doReload) {
    loadDB();
    citationIDs.clear();
  }
  
  if (citationIDs.empty()) {
    buildCitationRun();
  }
  
  // re-check the queue
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
  
  ofLog() << "Scheduling latest user comment";
  
  // alternatively, pop deque until front is non-comment,
  // then push_front the last comment (the new one)
  
  // brute force empty until last comment is in front
  while (citationIDs.front() != db.numRows-1 and !citationIDs.empty()) {
    citationIDs.pop_front();
  }
  
  // check wether next is also a comment
  if (citationIDs.size() > 1) {
    deque<int>::iterator next = citationIDs.begin()+1;
    Citation * c = Citation::fromCSVRow(db, *next);
    if (c->isComment()) {
      citationIDs.erase(next);
    }
    delete c;
  }

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
