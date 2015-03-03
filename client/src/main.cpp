#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char *argv[] ){
  
  // check if launched from Xcode
  if (argc > 1 && strcmp(argv[1], "-windowed") == 0) {
    ofSetupOpenGL(1366,768,OF_WINDOW);			// <-------- setup the GL context
  } else {
    ofSetupOpenGL(1366,768,OF_FULLSCREEN);			// <-------- setup the GL context
  }

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
