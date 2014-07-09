#pragma once

#include "ofMain.h"
#include "ofxSimpleTimer.h"

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
    
        ofTrueTypeFont futura60;
        ofTrueTypeFont futura24;
    
        string satz1;
        string satz2;
    
        string satz1full;
        string satz2full;
    
    float text_position;
    int last_text_pos;
    int cite_index, author_index;
    
    
    ofxSimpleTimer zitateTimer;
    ofxSimpleTimer buchstabenTicker;
		
    
    void zitateTimerStartHandler(int &args);
};
