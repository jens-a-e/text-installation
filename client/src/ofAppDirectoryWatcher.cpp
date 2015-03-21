//
//  ofAppDirectoryWatcher.cpp
//  CiteClient
//
//  Created by jens on 20.03.15.
//
//
#include "ofApp.h"


void ofApp::onDirectoryWatcherItemAdded(const DirectoryWatcherManager::DirectoryEvent& evt)
{
  ofSendMessage("Added:    " + evt.item.path());
}

void ofApp::onDirectoryWatcherItemRemoved(const DirectoryWatcherManager::DirectoryEvent& evt)
{
  ofSendMessage("Removed:  " + evt.item.path());
}

void ofApp::onDirectoryWatcherItemModified(const DirectoryWatcherManager::DirectoryEvent& evt)
{
  string filename = evt.item.path();
  if (filename == dbPath) {
    // DB has been modified
    // scheduleReload();
  }
}

void ofApp::onDirectoryWatcherItemMovedFrom(const DirectoryWatcherManager::DirectoryEvent& evt)
{
  ofLogNotice("ofApp::onDirectoryWatcherItemMovedFrom") << "Moved From: " << evt.item.path();
}

void ofApp::onDirectoryWatcherItemMovedTo(const DirectoryWatcherManager::DirectoryEvent& evt)
{
  ofLogNotice("ofApp::onDirectoryWatcherItemMovedTo") << "Moved To: " << evt.item.path();
}

void ofApp::onDirectoryWatcherError(const Poco::Exception& exc)
{
  ofLogError("ofApp::onDirectoryWatcherError") << "Error: " << exc.displayText();
}
