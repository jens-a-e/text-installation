#pragma once

#include "ofMain.h"
#include "ofxCsv.h"
#include "ofxSimpleTimer.h"
#include "ofxTextSuite.h"
#include "ofxTrueTypeFontUL2.h"
#include "ofxXmlSettings.h"

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

    wstring satz1a, satz1b, satz1c;
    wstring satz2a, satz2b;

    wstring satz1afull, satz1bfull, satz1cfull;
    wstring satz2afull, satz2bfull;
  
  bool bitmapRendering = true;
  int align;
    
    float text_position, timer;
    int last_text_pos;
    int cite_index, author_index, author2_index;
    int alpha;
    
    
    ofxSimpleTimer zitateTimer;
    ofxSimpleTimer buchstabenTicker;
    ofxSimpleTimer clearTimer;
    
    TextBlockAlignment alignment;
    ofxTextBlock autor1, autor2;
		
    
    void zitateTimerStartHandler(int &args);
    void zitateTimerCompleteHandler(int &args);
    
    void clearTimerStartHandler(int &args);
    void clearTimerCompleteHandler(int &args);
};
