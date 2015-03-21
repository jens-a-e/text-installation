//
//  ofAppEvents.cpp
//  CiteClient
//
//  Created by jens on 20.03.15.
//
//

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  string strDirection = "";
  string strAlign = "";
  switch(key){
    case '#':
      bAlignByPixel = !bAlignByPixel;
      mainFace.setAlignByPixel(bAlignByPixel);
      mainFaceLarge.setAlignByPixel(bAlignByPixel);
    case 'w':
    case 'W':
      mainFace.setWordWrap(!mainFace.getWordWrap());
      break;
    case 'q':
    case 'Q':
      mainFace.setUseLayoutCache(!mainFace.getUseLayoutCache());
      break;
    case 'e':
    case 'E':
      mainFace.setAlignByPixel(!mainFace.getAlignByPixel());
      break;
    case '0':
      align = UL2_TEXT_ALIGN_INVALID;
      strAlign="No Alignment [0]";
      break;
    case '1':
      align = UL2_TEXT_ALIGN_V_BOTTOM|UL2_TEXT_ALIGN_LEFT;
      strAlign="bottom-left [1]";
      break;
    case '2':
      align = UL2_TEXT_ALIGN_V_BOTTOM|UL2_TEXT_ALIGN_CENTER;
      strAlign="bottom-center [2]";
      break;
    case '3':
      align = UL2_TEXT_ALIGN_V_BOTTOM|UL2_TEXT_ALIGN_RIGHT;
      strAlign="bottom-right [3]";
      break;
    case '4':
      align = UL2_TEXT_ALIGN_V_MIDDLE|UL2_TEXT_ALIGN_LEFT;
      strAlign="middle-left [4]";
      break;
    case '5':
      align = UL2_TEXT_ALIGN_V_MIDDLE|UL2_TEXT_ALIGN_CENTER;
      strAlign="middle-center [5]";
      break;
    case '6':
      align = UL2_TEXT_ALIGN_V_MIDDLE|UL2_TEXT_ALIGN_RIGHT;
      strAlign="middle-right [6]";
      break;
    case '7':
      align = UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_LEFT;
      strAlign="top-left [7]";
      break;
    case '8':
      align = UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_CENTER;
      strAlign="top-center [8]";
      break;
    case '9':
      align = UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_RIGHT;
      strAlign="top-right [9]";
      break;
    case 'a':
    case 'A':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_LTR,UL2_TEXT_DIRECTION_TTB);
      strDirection="Left to Right (TTB)";
      break;
    case 's':
    case 'S':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_RTL,UL2_TEXT_DIRECTION_TTB);
      strDirection="Right to Left (TTB)";
      break;
    case 'd':
    case 'D':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_TTB,UL2_TEXT_DIRECTION_LTR);
      strDirection="Top to Bottom (LTR)";
      break;
    case 'f':
    case 'F':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_BTT,UL2_TEXT_DIRECTION_LTR);
      strDirection="Bottom to Top (LTR)";
      break;
    case 'z':
    case 'Z':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_LTR,UL2_TEXT_DIRECTION_BTT);
      strDirection="Left to Right (BTT)";
      break;
    case 'x':
    case 'X':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_RTL,UL2_TEXT_DIRECTION_BTT);
      strDirection="Right to Left (BTT)";
      break;
    case 'c':
    case 'C':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_TTB,UL2_TEXT_DIRECTION_RTL);
      strDirection="Top to Bottom (RTL)";
      break;
    case 'v':
    case 'V':
      mainFace.setTextDirection(UL2_TEXT_DIRECTION_BTT,UL2_TEXT_DIRECTION_RTL);
      strDirection="Bottom to Top (RTL)";
      break;
    case '.':
      bDebug = !bDebug;
      break;
      
  }
  
  ofLog(OF_LOG_NOTICE, strAlign);
  ofLog(OF_LOG_NOTICE, strDirection);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
  
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
  
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
  
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
  
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
  
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
  ofLog() << msg.message;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
  
}