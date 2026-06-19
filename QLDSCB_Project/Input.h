#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include "UI.h"
#include <string>

using namespace std;

void _TruyenThamSo(listMB &A, listCB& B , listHK &C, listSBD& D);

void RedrawInput(string result, int x, int y, int maxLength, char placeholder, bool isCenter,bool isHighlight,string SugestionStr = "");
int  InputStatus(int &result, int x, int y);
int  InputString(string &result, int x, int y, int maxLength, char placeholder, bool onlyNumbers = false, bool isCenter = false,bool isHighlight = false, bool isUppercase = false,bool isonlychar = true,bool canspace = true);
int  InputDateTime(string &result, int x, int y,int maxLength, char placeholder, bool isCenter = false, bool isHighlight = false, int errX =-1, int errY=-1) ;
int  InputStringSBD(string &result, int x, int y, int maxLength, char placeholder, bool isCenter = false, bool isHighlight = false);
bool InputSimpleHK(string& res, NavKey key, int& ch,int x,int y);
bool SimpleInput(string& res, NavKey key, int& ch, int x, int y,int width,bool canspace = false, bool onlyNums = false);

#endif