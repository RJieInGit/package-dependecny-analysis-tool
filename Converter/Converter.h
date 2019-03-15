#pragma once
///////////////////////////////////////////////////////////////////////////
// Converter.h   : defines source code conversion to webpage functions   //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Pj2 Converter from analysis result to web Page//                          
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Ammar Salman, Syracuse University//
//                 313/788-4694, assalman@syr.edu       
//
//                 Jie Ren, Syracuse University   //adding htmlmark up and dependecny analysis
//                 3152890047, jren21@syr.edu 
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodeConverter class which accepts DependencyTable
*  and uses its relationships to create linked webpages that point to each
*  other based on dependencies. It can also convert single files if needed.
*  The conversion process filters HTML special characters before printing
*  them into output files. The resulting output of this converter is a list
*  of files (vector<string>) of the created webpages. 

           This package also provide a facility to add javascript and html markup to 
		   create a botton on the created web pages to hide/show the class,function,and namespace
*
*  Required Files:
* =======================
*  Converter.h Converter.cpp DependencyTable.h DependencyTable.cpp
*  Logger.h Logger.cpp FileSystem.h FileSystem.cpp
*
*  Maintainence History:
* =======================
   ver 2.0 -03/06 2019 by Jie Ren : add functions to add buttons and html mark ups
   
*  ver 1.0 - 11 Feb 2019
*  - first release
*/

#include "../DependencyTable/DependencyTable.h"
#include <vector>
#include <string>
#include <fstream>
#include<unordered_set>
using namespace std;
class CodeConverter
{
public:
  CodeConverter();
  CodeConverter(const DependencyTable& dt);

  const std::string& outputDir() const;
  void outputDir(const std::string& dir);
  bool createOutpurDir();

  void setDepTable(const DependencyTable& dt);

  std::vector<std::string> convert();
  std::string convert(const std::string& filepath);
  std::vector<std::string> convert(const std::vector<std::string>& files);
  
  const std::vector<std::string> convertedFiles() const;

  void clear();
  std::vector<std::string> convertFromDir(std::string,std::string);

bool convertFile(std::string file);
std::string addPreComment(std::string line);
private:
	bool inMutiComment = false;
	bool isPrePage = true;
  void addPreCodeHTML(const std::string& title);
  void addPreTag();
  void addButton();
  void addScript();
  void addDependencyLinks(std::string file);
  void addClosingTags();
  void skipSpecialChars(std::string& line);
  void getInfoFromAST(std::string);
  std::string  addmarkup(string, int);
private:
  DependencyTable dt_;
  std::string outputDir_ = "..\\ConvertedWebpages\\";
  std::vector<std::string> convertedFiles_;
  std::ofstream out_;
  std::unordered_set<int> endDiv; 
  std::unordered_set<int> classdiv; 
  std::unordered_set<int> functiondiv;
 
};

