#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char *argv[] ){
  
  // check if launched from Xcode
  if (argc > 1 && strcmp(argv[1], "-windowed") == 0) {
    ofSetupOpenGL(1366, 768, OF_WINDOW);
  } else {
    ofSetupOpenGL(1366, 768, OF_FULLSCREEN);
  }

	ofRunApp(new ofApp());

}
