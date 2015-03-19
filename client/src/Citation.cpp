//
//  Citation.cpp
//  CiteClient
//
//  Created by jens on 19.03.15.
//
//

#include "Citation.h"


Citation::Citation(int id, wstring body, wstring author, wstring affiliation, int year, wstring reason) {
  this->id = id;
  this->body = body;
  this->author = author;
  this->affiliation = affiliation;
}

Citation::~Citation(){
  
}

const Citation Citation::fromCSVRow(wng::ofxCsv csv, int row) {
  // Nummer,Zitat,Name,Position,Jahr,Anlass
  
  row += 1; // the first row is a table header
  string body, author, affiliation, reason;
  
  int id   = csv.getInt(row, 0);
  body     = csv.getString(row, 1);
  author   = csv.getString(row, 2);
  affiliation   = csv.getString(row, 3);
  int year = csv.getInt(row, 4);
  reason   = csv.getString(row, 3);
  
  return Citation(
                  id,
                  wstring(*body.c_str(),body.length()),
                  wstring(*author.c_str(),author.length()),
                  wstring(*affiliation.c_str(),affiliation.length()),
                  year,
                  wstring(*reason.c_str(),reason.length())
  );
}