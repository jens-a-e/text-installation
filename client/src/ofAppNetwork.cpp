//
//  ofAppNetwork.cpp
//  CiteClient
//
//  Created by jens on 20.03.15.
//
//

#include "ofApp.h"


void ofApp::setupNetwork() {
  setupClientNetwork();
  setupMasterConnection();
}

void ofApp::setupClientNetwork() {
  clientNet = ofxUDPManager();
  clientNet.Create();
  clientNet.SetEnableBroadcast(true);
  clientNet.SetNonBlocking(true);
  clientNet.SetReuseAddress(true);
  clientNet.Bind(Settings.getValue("udp:client", 3331));
}

void ofApp::broadCastClients(string msg) {
  ofLog(OF_LOG_NOTICE,"sending to client net: "+msg);
  clientNet.Close();
  clientNet = ofxUDPManager();
  clientNet.Create();
  clientNet.SetEnableBroadcast(true);
  clientNet.SetNonBlocking(true);
  clientNet.SetReuseAddress(true);
  clientNet.Connect("255.255.255.255", Settings.getValue("udp:client", 3331));
  clientNet.Send(msg.c_str(), msg.size());
  clientNet.Close();
  setupClientNetwork();
}

void ofApp::clientNetworkUpdate() {
  int buff_size = clientNet.GetReceiveBufferSize();
  if (buff_size > 0) {
    char* buffer = (char*) malloc(buff_size * sizeof(char));
    clientNet.Receive(buffer, buff_size);
    string message = buffer;
    free(buffer);
    if(message == "") return;
    ofLog(OF_LOG_NOTICE) << "Client Network recieved: " << message;
    if(message.find("citing:") != std::string::npos) {
      ofStringReplace(message, "citing:", "");
      popCitation(ofToInt(message));
      DumpCitationList();
    }
    if(message.find("commenting:") != std::string::npos) {
      ofStringReplace(message, "citing:", "");
      popComment(ofToInt(message));
      DumpCitationList();
    }
  }
}

void ofApp::setupMasterConnection() {
  masterConnection.Create();
  masterConnection.SetNonBlocking(true);
  masterConnection.Bind(Settings.getValue("udp:master", 3332));
}

void ofApp::masterConnectionUpdate() {
  char udpMessage[1024];
  masterConnection.Receive(udpMessage,1024);
  string message=udpMessage;
  if (message != "") {
    ofLog() << "Master Network send: " << message;
    if (message == "update") {
      scheduleDownload();
    }
    else if (message == "showUserComment") {
      doDownloadComments = true;
      scheduleUserComment();
    }
  }
}

void ofApp::scheduleDownload() {
  doDownloadCites = doDownloadComments = true;
}

bool ofApp::downloadCites() {
  if(!doDownloadCites) return true;
  
  string URL = Settings.getValue("urls:zitate", "http://master.text:4200/zitate.csv");

  ofHttpResponse resp = ofSaveURLTo(URL, ofToDataPath("downloaded-zitate.csv"));
  
  if (resp.status != 200) {
    ofLog() << "Error updating DB " << resp.error;
    return false;
  } else {
    ofLog() << "Updated citation DB";
    ofFile::copyFromTo("zitate.csv", "zitate.csv.bak", true, true);
    ofFile::copyFromTo("downloaded-zitate.csv", "zitate.csv", true, true);
    ofFile::removeFile("downloaded-zitate.csv");
    scheduleReload();
    doDownloadCites = false;
    return true;
  }
}


bool ofApp::downloadComments() {
  if(!doDownloadComments) return true;
  
  string URL = Settings.getValue("urls:comments", "http://master.text:4200/comments.csv");
  
  ofHttpResponse resp = ofSaveURLTo(URL, ofToDataPath("downloaded-comments.csv"));
  
  if (resp.status != 200) {
    ofLog() << "Error updating DB " << resp.error;
    return false;
  } else {
    ofLog() << "Updated comments DB";
    ofFile::copyFromTo("comments.csv", "comments.csv.bak", true, true);
    ofFile::copyFromTo("downloaded-comments.csv", "comments.csv", true, true);
    ofFile::removeFile("downloaded-comments.csv");
    scheduleReload();
    doDownloadComments = false;
    return true;
  }
}