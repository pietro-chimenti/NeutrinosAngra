//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraConstantMgr implementation file
//
//  Authors: P.Chimenti 
//
//  25-3-2010, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//==============================================================
//In this file is the class responsible for logging the simulation
//results
//==============================================================

#include "AngraConstantMgr.hh"
#include <climits>
using namespace std;

bool AngraConstantMgr::setup=false;

bool AngraConstantMgr::Init()
{
  inFile = new std::ifstream();
// inFile->open("constantsCopia.dat");
 inFile->open("constants.dat");

  std::string key;

  float value;
  (*inFile) >> key;
  while(key.length()>0){
    (*inFile) >> value;
    constants[key]=value;  
    (*inFile).ignore(INT_MAX, '\n');
    key.clear();
    (*inFile) >> key;
  }
  return true;
}

float AngraConstantMgr::GetValue(std::string key)
{
return constants[key];
}

