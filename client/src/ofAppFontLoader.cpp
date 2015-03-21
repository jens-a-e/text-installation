//
//  ofAppFontLoader.cpp
//  CiteClient
//
//  Created by jens on 21.03.15.
//
//

#include "ofApp.h"

void ofApp::loadFonts() {
  
  string bookObliqueFont  = ofToDataPath("FuturaStd-BookOblique.otf");
  string heavyFont        = ofToDataPath("FuturaStd-Heavy.otf");
  string heavyObliqueFont = ofToDataPath("FuturaStd-HeavyOblique.otf");
  
  
  mainFace.setLetterSpacing(Settings.getValue("settings:fonts:main:letter-spacing", 0.1));
  mainFace.setLineHeight(Settings.getValue("settings:fonts:main:line-height", 100));

  mainFace.setWordWrap(true);
  mainFace.setAlignByPixel(bAlignByPixel);
  mainFace.setUseLayoutCache(true);
  
  mainFace.loadFont(heavyFont, Settings.getValue("settings:fonts:main:size", 68), true, true);
  
  
  mainFaceLarge.setLetterSpacing(Settings.getValue("settings:fonts:main-large:letter-spacing", 0.1));
  mainFaceLarge.setLineHeight(Settings.getValue("settings:fonts:main-large:line-height", 100));
  
  mainFaceLarge.setWordWrap(true);
  mainFaceLarge.setAlignByPixel(bAlignByPixel);
  mainFaceLarge.setUseLayoutCache(true);
  
  mainFaceLarge.loadFont(heavyFont, Settings.getValue("settings:fonts:main-large:size", 68), true, true);
  
  
  citeNameFace.setLineHeight(44);
  citeNameFace.setWordWrap(false);
  citeNameFace.setUseLayoutCache(true);

  citeNameFace.loadFont(heavyObliqueFont, 34);
  
  
  citeMetaFace.setLineHeight(44);
  citeMetaFace.setWordWrap(false);
  citeMetaFace.setUseLayoutCache(true);

  citeMetaFace.loadFont(bookObliqueFont, 34);
  
}