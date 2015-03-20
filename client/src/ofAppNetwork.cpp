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
  clientNet.SetEnableBroadcast(true);
  clientNet.SetNonBlocking(true);
  clientNet.SetReuseAddress(true);
  clientNet.Bind(3331);
  clientNet.Create();
}

void ofApp::broadCastClients(string msg) {
  // TODO: Use IP address from a settings file, or try to get from system call
  clientNet.Connect("192.168.123.255", 3331);
  clientNet.SendAll(msg.c_str(), msg.size());
}

void ofApp::clientNetworkUpdate() {
  
}

void ofApp::setupMasterConnection() {
  
}

void ofApp::masterConnectionUpdate() {
  
}