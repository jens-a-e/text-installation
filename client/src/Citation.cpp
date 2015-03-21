//
//  Citation.cpp
//  CiteClient
//
//  Created by jens on 19.03.15.
//
//

#include "Citation.h"


Citation::Citation(int id, string body, string author, string affiliation, string year, string reason) {
  this->id = id;
  this->body = body;
  this->author = author;
  this->affiliation = affiliation;
  this->year = year;
  this->reason = reason;
}

Citation::~Citation(){
  
}

Citation* Citation::fromCSVRow(wng::ofxCsv csv, int row) {
  // [Row = ID]: Zitat,Name,Position,Jahr,Anlass
  
  row += 1; // the first row is a table header
  string body, author, affiliation, reason, year;
  
  int id        = row;
  body          = csv.getString(row, 0);
  author        = csv.getString(row, 1);
  affiliation   = csv.getString(row, 2);
  year          = csv.getString(row, 3);
  reason        = csv.getString(row, 4);

  size_t pos;
  string newline = "";
  newline += '\n';
  while((pos = body.find("\\n")) != string::npos) {
    stringstream _body;
    _body << body.substr(0, pos) << std::endl << body.substr(pos+2,body.length());
    body = _body.str();
  }
  
  return new Citation(
                  id,
                  body,
                  author,
                  affiliation,
                  year,
                  reason
  );
}

const string Citation::toString() {
  return "ID: "+ofToString(id)
  +"\nBody: "+body
  +"\nAuthor: "+author
  +"\nAffilitation: "+affiliation
  +"\nYear: "+year
  +"\nReason: "+reason
  +"\n";
}

