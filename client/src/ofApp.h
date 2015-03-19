#pragma once

#include "ofMain.h"
#include "ofxCsv.h"
#include "ofxSimpleTimer.h"
#include "ofxTextSuite.h"
#include "ofxTrueTypeFontUL2.h"
#include "ofxXmlSettings.h"
#include "Citation.h"

// for ofxCsv:
using namespace wng;
using namespace std;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

  // We want to use Unicode features to be safe and
  // get better text layout with harfbuzz
    ofxTrueTypeFontUL2 mainFace, citeNameFace, citeMetaFace, utilFace;

    string satz1a, satz1b, satz1c;

    string satz1afull, satz1bfull, satz1cfull;
  
    Citation *currentCitation;
    
    bool bitmapRendering = true;
    int align;
    
    float text_position, timer;
    int last_text_pos;
    int cite_index, author_index, author2_index;
    int alpha;
  
  
    ofxSimpleTimer clearTimer;
    ofxSimpleTimer zitateTimer, rewindTimer;
  
    TextBlockAlignment alignment;
    ofxTextBlock autor1, autor2;
  
    void zitateTimerStartHandler(int &args);
    void zitateTimerCompleteHandler(int &args);
  
  void rewindTimerStartHandler(int &args);
  void rewindTimerCompleteHandler(int &args);
  
    void clearTimerStartHandler(int &args);
    void clearTimerCompleteHandler(int &args);
  

  // Citation DB
  ofxCsv db;
  void loadDB();
  
  int lastCitationID;
  int currentCitationID();
  std::stack<int,std::vector<int> > * citationIDs;
  
  void buildCitationRun();
  void nextCitation();
};
