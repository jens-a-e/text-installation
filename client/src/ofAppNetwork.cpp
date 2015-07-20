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
  clientNet.Create();
  clientNet.SetEnableBroadcast(true);
  clientNet.SetNonBlocking(true);
  clientNet.SetReuseAddress(true);
  clientNet.Bind(3331);
}

void ofApp::broadCastClients(string msg) {
  clientNet.Close();
  clientNet.Connect("255.255.255.255", 3331);
  clientNet.SendAll(msg.c_str(), msg.size());
  clientNet.Close();
  setupClientNetwork();
}

void ofApp::clientNetworkUpdate() {
  int buff_size = clientNet.GetReceiveBufferSize();
  if (buff_size > 0) {
    char* buffer = (char*) malloc(buff_size * sizeof(char));
    clientNet.Receive(buffer, buff_size);
    string message = string(buffer);
    free(buffer);
    if(message == "") return;
    ofLog() << "Client Network recieved: " << message;
    if(message.find("citing:") != std::string::npos) {
      ofStringReplace(message, "citing:", "");
      char _remote[64];
      clientNet.GetRemoteAddr(_remote);
      string remote(_remote);
      int cited = ofToInt(message);
      
      clientCites[remote] = cited;
      
      std::cout << remote << " send: " << message << ", the map is now:\n";
      for (std::map<string,int>::iterator it=clientCites.begin(); it!=clientCites.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';
    }
  }
}

void ofApp::setupMasterConnection() {
  masterConnection.Create();
//  masterConnection.SetEnableBroadcast(true);
  masterConnection.SetNonBlocking(true);
  masterConnection.Bind(3332);
}

void ofApp::masterConnectionUpdate() {
  char udpMessage[1000];
  masterConnection.Receive(udpMessage,1000);
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
  ofHttpResponse resp = ofSaveURLTo("http://master.text:4200/zitate.csv", ofToDataPath("downloaded-db.csv"));
  
  if (resp.status != 200) {
    ofLog() << "Error updating DB " << resp.error;
  } else {
    ofLog() << "Updated DB" << resp.error;
    ofFile::copyFromTo("zitate.csv", "zitate.csv.bak", true, false);
    ofFile::copyFromTo("downloaded-db.csv", "zitate.csv", true, true);
    scheduleReload();
  }
}