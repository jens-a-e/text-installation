#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( int argc, char *argv[] ){
  ofAppGLFWWindow window;
  window.setNumSamples(8);
  window.setDoubleBuffering(true);
  window.setMultiDisplayFullscreen(false);
  
  // check if launched from Xcode
  if (argc > 1 && strcmp(argv[1], "-windowed") == 0) {
    ofSetupOpenGL(&window, 1366, 768, OF_WINDOW);
  } else {
    ofSetupOpenGL(&window, 1366, 768, OF_FULLSCREEN);
  }

	ofRunApp(new ofApp());

}
