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
  string body,author,affiliation,year,reason;
  int id;
  
  Citation(int id, string body, string author, string affiliation, string year, string reason);
  ~Citation();
  
  static const Citation fromCSVRow(ofxCsv csv, int row);

  const string toString();
};

#endif /* defined(__CiteClient__Citation__) */
