//
//  Citation.h
//  CiteClient
//
//  Created by jens on 19.03.15.
//
//

#ifndef __CiteClient__Citation__
#define __CiteClient__Citation__

#include <locale>
#include <string>
#include "ofxCsv.h"

using namespace std;
using namespace wng;

class Citation {
public:
  wstring body,author,affiliation;
  int id, year;
  
  Citation(int id, wstring body, wstring author, wstring affiliation, int year, wstring reason);
  ~Citation();
  
  static const Citation fromCSVRow(ofxCsv csv, int row);
};

#endif /* defined(__CiteClient__Citation__) */
