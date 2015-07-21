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
  commentsDb.loadFile(commentsPath);
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
  bool newCites = false; bool newComments = false;
  
  if(doDownloadCites) {
    newCites = downloadCites();
    if (newCites) {
      ofLog() << "Downloaded new citations.";
    } else {
      ofLog() << "Could not download new citations.";
    }
    // check wether to retry next time!
    doDownloadCites = !newCites;
  }
  
  if(doDownloadComments) {
    bool newComments = downloadComments();
    if (newComments) {
      ofLog() << "Downloaded new comments.";
    } else {
      ofLog() << "Could not download new comments.";
    }
    // unflag new comment signal, comment-download failed
    mustBeComment = newComments & mustBeComment;
    // check wether to retry next time!
    doDownloadComments = !newComments;
  }
  
  if (newCites || newComments) {
    // reload on any successful download
    doReload = true;
  }
  
  if (doReload) {
    loadDB();
    citationIDs.clear();
    citations.clear();
    comments.clear();
  }
  
//  if (citationIDs.empty()) {
//    buildCitationRun();
//  }
//
  ofLog() << "Cites in buffer " << citations.size() << " " << citations.empty();
  ofLog() << "Comments in buffer " << comments.size() << " " << comments.empty();
  
  if (citations.empty()) {
    citations.clear();
    ofLog() << "rows in citations: " << db.numRows;
    for (int r=1; r <= db.numRows-1; r++) {
      citations.push_back(r);
    }
  }
  if (comments.empty()) {
    comments.clear();
    ofLog() << "rows in comments: " << commentsDb.numRows;
    for (int r=0; r < commentsDb.numRows; r++) {
      comments.push_back(r);
    }
  }
  ofLog() << "Cites in buffer " << citations.size() << " " << citations.empty();
  ofLog() << "Comments in buffer " << comments.size() << " " << comments.empty();
  
  // schedule download + reload, if still empty
  if(citations.empty() || comments.empty()) {
    scheduleDownload();
    scheduleReload();
  }
  
  // re-check the queue
//  if (citationIDs.empty()) {
//    ofLog(OF_LOG_NOTICE) << "No ids to cite";
//    return;
//  }

  if (citations.empty() && comments.empty()) {
    ofLog(OF_LOG_NOTICE) << "No citations loaded";
    return;
  }
  
  ofLog() << "Before pick:";
  DumpCitationList();

//  int next = citationIDs.front();
//  citationIDs.pop_front();
//  
//  if(currentCitation != NULL) {
//    delete currentCitation;
//  }
//
//  currentCitation = Citation::fromCSVRow(db, next);
//  
//  ofLog() << currentCitation->toString();

  if (numComments+numCites >= 7) {
    numCites = numComments = 0;
  }
  
  ofSeedRandom();

  bool noCites = citations.empty();
  bool noComments = comments.empty();
  
  bool pickCite = ofRandomf() > 1/4.f;
  
  if (numComments >= 1) {
    pickCite = !noCites;
  }
  else if (numCites >= 6) {
    pickCite = !noComments;
  }
  else if(mustBeComment) {
    pickCite = false;
  }
  
  int nextId = -1;

  if (pickCite && !noCites) {
    numCites = MIN(6,numCites+1);
//    if (!citations.empty()) {
      int pos = ofRandom(citations.size()-1);
      list<int>::iterator item = citations.begin();
      advance(item, pos);
      nextId = *item;
      citations.erase(item);
//    }
  }
  else if(!pickCite && !noComments) {
    numComments = MIN(1,numComments+1);
//    if(!comments.empty()) {
    list<int>::iterator item;
    if (mustBeComment) {
      nextId = comments.back();
      comments.pop_back();
    } else {
      int pos = ofRandom(comments.size()-1);
      item = comments.begin();
      advance(item, pos);
      nextId = *item;
      comments.erase(item);
    }
//    }
  }

  ofLog() << "Current stats: " << numComments << " to " << numCites;
  
  if(currentCitation != NULL) {
    delete currentCitation;
  }

  if(nextId >= 0) {
    currentCitation = Citation::fromCSVRow(pickCite ? db : commentsDb, nextId);
  } else {
    currentCitation = NULL;
  }

  if(currentCitation != NULL) {
    ofLog() << "Current citation:" << endl << currentCitation->id;
  
   notifyCiting(currentCitation);
  }
  
  ofLog() << "After pick:";
  DumpCitationList();
}

bool ofApp::popCitation(int id){
  if (citations.empty()) return true;
  bool found = false;
  unsigned int limit = 0;
  for(list<int>::iterator i = citations.begin(); i != citations.end(); ++i) {
    if(++limit == 10) break;
    if (id == *i) {
      citations.erase(i);
      found = true;
      break;
    }
  }
  return found;
}

bool ofApp::popComment(int id){
  if (comments.empty()) return true;
  bool found = false;
  unsigned int limit = 0;
  for(list<int>::iterator i = comments.begin(); i != comments.end(); ++i) {
    if(++limit == 10) break;
    if (id == *i) {
      comments.erase(i);
      found = true;
      break;
    }
  }
  return found;
}

void ofApp::notifyCiting(Citation *c) {
  // update the
  // TODO add/switch on type
  if(c->isComment()) {
    broadCastClients("commenting:"+ofToString(c->id));
  } else {
    broadCastClients("citing:"+ofToString(c->id));
  }
}

void ofApp::scheduleUserComment() {
  mustBeComment = true;
}


void ofApp::DumpCitationList(){
  ofLog(OF_LOG_NOTICE) << "---------------";

  stringstream ids;
  int count = 0;
  for(list<int>::iterator i = citations.begin(); i != citations.end(); ++i) {
    ids << *i << "\t";
    if(++count%10 == 0) {
      ids << endl;
    }
  }
  ofLog(OF_LOG_NOTICE) << "Current citation ids:" << endl << ids.str();

  stringstream cIds;
  count = 0;
  for(list<int>::iterator i = comments.begin(); i != comments.end(); ++i) {
    cIds << *i << "\t";
    if(++count%10 == 0) {
      cIds << endl;
    }
  }
  ofLog(OF_LOG_NOTICE) << "Current comment ids:" << endl << cIds.str();
  
  ofLog(OF_LOG_NOTICE) << "---------------";
}
