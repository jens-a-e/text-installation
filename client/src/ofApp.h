#pragma once

#include "ofMain.h"
#include "ofxSimpleTimer.h"
#include "ofxTextSuite.h"

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
    
        ofTrueTypeFont futura68, futura34, futura34b;
    
        string satz1a, satz1b, satz1c;
        string satz2a, satz2b;
    
        string satz1afull, satz1bfull, satz1cfull;
        string satz2afull, satz2bfull;;
    
    
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
