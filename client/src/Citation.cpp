//
//  Citation.cpp
//  CiteClient
//
//  Created by jens on 19.03.15.
//
//

#include "Citation.h"
using namespace std;
using namespace wng;

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

void cleanBody(string *body) {
  if(body == NULL) return;
  size_t pos;
  string newline = "";
  newline += '\n';
  while((pos = body->find("\\n")) != string::npos) {
    stringstream _body;
    _body << body->substr(0, pos) << std::endl << body->substr(pos+2,body->length());
    *body = _body.str();
  }
}

Citation* Citation::fromCSVRow(ofxCsv csv, int row) {
  // [Row = ID]: Zitat,Name,Position,Jahr,Anlass
  
  string body, author, affiliation, reason, year;
  
  body          = csv.getString(row, 0);
  author        = csv.getString(row, 1);
  affiliation   = csv.getString(row, 2);
  year          = csv.getString(row, 3);
  reason        = csv.getString(row, 4);

  cleanBody(&body);
  return new Citation(
                  row,
                  body,
                  author,
                  affiliation,
                  year,
                  reason
  );
}

const bool Citation::isComment(){
  return reason == "comment";
}

Citation* Citation::fromString(string s) {
  ofxCsv csv;
  stringstream stream;
  
  vector<vector<string> > lines;
  
  stream << s;
  
  csv.ParseCSVString(stream,lines);
  
  if (lines.size() == 0 || lines.at(0).size() < 5) {
    return NULL;
  }
  
  vector<string> data = lines.at(0);

  int id = -1;
  string body, author, affiliation, reason, year;
  
  body          = data.at(0);
  author        = data.at(1);
  affiliation   = data.at(2);
  year          = data.at(3);
  reason        = data.at(4);
  cleanBody(&body);

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
  +"\nReason: "+reason;
}

