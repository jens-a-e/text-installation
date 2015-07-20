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
  clientNet.Bind(3331);
}

void ofApp::broadCastClients(string msg) {
  ofLog(OF_LOG_NOTICE,"sending to client net: "+msg);
  clientNet.Close();
  clientNet = ofxUDPManager();
  clientNet.Create();
  clientNet.SetEnableBroadcast(true);
  clientNet.SetNonBlocking(true);
  clientNet.SetReuseAddress(true);
  clientNet.Connect("255.255.255.255", 3331);
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
  }
}

void ofApp::setupMasterConnection() {
  masterConnection.Create();
  masterConnection.SetNonBlocking(true);
  masterConnection.Bind(3332);
}

void ofApp::masterConnectionUpdate() {
  char udpMessage[1024];
  masterConnection.Receive(udpMessage,1024);
  string message=udpMessage;
  if (message != "") {
    ofLog() << "Master Network send: " << message;
    if (message == "update") {
      // load new db file
      scheduleDownload();
    }
    else if (message == "showUserComment") {
      scheduleUserComment();
    }
  }
}

void ofApp::scheduleDownload() {
  if (!doDownload) {
    ofLog() << "DB already downloaded this frame.";
    return;
  }
  ofHttpResponse resp = ofSaveURLTo("http://master.text:4200/zitate.csv", ofToDataPath("downloaded-db.csv"));
  
  if (resp.status != 200) {
    ofLog() << "Error updating DB " << resp.error;
  } else {
    ofLog() << "Updated DB" << resp.error;
    ofFile::copyFromTo("zitate.csv", "zitate.csv.bak", true, false);
    ofFile::copyFromTo("downloaded-db.csv", "zitate.csv", true, true);
    scheduleReload();
    doDownload = false;
  }
}