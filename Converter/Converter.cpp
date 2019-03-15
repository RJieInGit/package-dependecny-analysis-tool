///////////////////////////////////////////////////////////////////////////
// Converter.cpp : implements and provides test stub for Converter.h     //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Ammar Salman, Syracuse University                     //
//                 313/788-4694, assalman@syr.edu         
//                 Jie Ren,     Syracuse University
//                 315/289-0047  jren21@syr.edu
///////////////////////////////////////////////////////////////////////////

#include "Converter.h"
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"
#include "../Dependency/dependency.h"
#include "../DependencyTable/DependencyTable.h"
#include <vector>
#include"../loader/loader.h"
using namespace FileSystem;
using namespace Logging;
using namespace std;
using namespace CodeAnalysis;
// -----< default ctor >--------------------------------------------------
CodeConverter::CodeConverter()
{
}

// -----< ctor to set dep table >-----------------------------------------
CodeConverter::CodeConverter(const DependencyTable & dt) : dt_(dt)
{
  //LoggerDebug::write("\n  CodeConverter initialized with DependencyTable\n");
}

// -----< set dependency table function >---------------------------------
void CodeConverter::setDepTable(const DependencyTable & dt)
{
  dt_ = dt;
}

// -----< convert pre-set dependency table >------------------------------
std::vector<std::string> CodeConverter::convert()
{
  if (!createOutpurDir()) {
   //LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
   // LoggerDebug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
    return std::vector<std::string>();
  }

  //LoggerDemo::write("\n\n  Converting files to webpages");
  //LoggerDebug::write("\n\n  Converting files to webpages");

  //LoggerDebug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
  //LoggerDemo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

  for (auto entry : dt_) {
    if (convertFile(entry.first)) {
      //LoggerDemo::write("\n  -- Converted: ");
     // LoggerDebug::write("\n  -- Converted: ");
    }
    else {
      //LoggerDemo::write("\n  -- Failed:    ");
      //LoggerDebug::write("\n  -- Failed:    ");
    }

    std::string filename = Path::getName(entry.first);
   // LoggerDebug::write(filename);
   // LoggerDemo::write(filename);
  }
 // LoggerDebug::write("\n");
  //LoggerDemo::write("\n");
  return convertedFiles_;
}

// -----< convert single file given path >----------------------------------
std::string CodeConverter::convert(const std::string & filepath)
{
  if (!createOutpurDir()) {
    //LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
    //LoggerDebug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
    return "";
  }

  //LoggerDemo::write("\n\n  Converting files to webpages");
  //LoggerDebug::write("\n\n  Converting files to webpages");

  //LoggerDebug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
  //LoggerDemo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

  if (convertFile(filepath)) {
   // LoggerDemo::write("\n  -- Converted: ");
    //LoggerDebug::write("\n  -- Converted: ");
  }
  else {
   // LoggerDemo::write("\n  -- Failed:    ");
   // LoggerDebug::write("\n  -- Failed:    ");
  }

  std::string filename = Path::getName(filepath);
 // LoggerDebug::write(filename);
  //LoggerDemo::write(filename);

  return filename + ".html";
}

std::vector<std::string> CodeConverter::convert(const std::vector<std::string>& files)
{
  clear();
  dt_ = DependencyTable(files);
  return convert();
}

// -----< get set output directory >----------------------------------------
const std::string & CodeConverter::outputDir() const
{
  return outputDir_;
}

// -----< set output directory >--------------------------------------------
void CodeConverter::outputDir(const std::string & dir)
{
  outputDir_ = dir;
}

// -----< create output directory >-----------------------------------------
/* returns success of creation. it could fail if dir is sys protected */
bool CodeConverter::createOutpurDir()
{
  if (!Directory::exists(outputDir_)) {
    //LoggerDebug::write("\n  Creating output directory. Path: " + Path::getFullFileSpec(outputDir_));
    return Directory::create(outputDir_);
  }
  //LoggerDebug::write("\n  Output directory already exists. Proceeding...");
  return true;
}

// -----< get list of converted files >-------------------------------------
const std::vector<std::string> CodeConverter::convertedFiles() const
{
  return convertedFiles_;
}

void CodeConverter::clear()
{
  convertedFiles_.clear();
  dt_.clear();
}

// -----< private - read file and create webpage >--------------------------    // modified
bool CodeConverter::convertFile(std::string file)
{    
	classdiv.clear();  functiondiv.clear(); endDiv.clear();
	isPrePage = true; inMutiComment = false;           cout << "\n now converting the file  " ;
  std::ifstream in(file);
  if (!in.is_open() || !in.good()) {
    in.close();
    return false;
  }
  std::string filename = Path::getName(file);
  std::string outputPath = outputDir_ + filename + ".html";           cout << "\n out put path is " << outputPath;
  out_.open(outputPath, std::ofstream::out);
  if (!out_.is_open() || !out_.good()) {
    in.close();
    return false;
  }  
  addPreCodeHTML(filename);          // write everything to file 
  addButton();
  addDependencyLinks(file);
  addPreTag();
  size_t count = 0;
  getInfoFromAST(file);
  while (in.good()) {
	  std::string line;
    while (std::getline(in, line)) {            //add logic to add html markups and 
		count++;
      out_ << addmarkup(line,count) << std::endl;
    }
  }
  out_ << "    </pre>" << std::endl;
  addScript();
  addClosingTags();
  out_.close();
  convertedFiles_.push_back(outputPath);
  return true;
}
//add div markup
string CodeConverter::addmarkup(string line,int count) {
	while (line.find('<') != line.npos)
		line.replace(line.find('<'), 1, "&lt");
	while (line.find('>') != line.npos)
		line.replace(line.find('>'), 1, "&gt");       // replace the  < > char in line to their escape sequence  <endl;
	if (classdiv.find(count) != classdiv.end())
		line = "<div class=\"isClass\">" + line;
	if (functiondiv.find(count) != functiondiv.end())
		line = "<div class=\"isFunction\">" + line;
	if (endDiv.find(count) != endDiv.end())
		line = line + "</div>";
	line = addPreComment(line);
	return line;
}

//get info from ASTtree
void CodeConverter::getInfoFromAST(string file) {
	queue <ASTNode*> q;
	q.push(dt_.getASTNode(file));
	while (q.size() > 0) {// using the information stored in the ASTtree
		ASTNode* node = q.front();
		if (node->type_ == "class") {
			classdiv.insert(node->startLineCount_);
			endDiv.insert(node->endLineCount_);
		}
		else if (node->type_ == "function"&&node->startLineCount_ - node->endLineCount_ > 1) {
			functiondiv.insert(node->startLineCount_);
			endDiv.insert(node->endLineCount_);
		}
		for (ASTNode* node1 : q.front()->children_) {
			q.push(node1);
		}
		q.pop();
	}
}
// add logic to add div to PrePage COmments
string CodeConverter:: addPreComment(string line) {
	if (!isPrePage)
		return line;
	if (inMutiComment) {
		if (line.find("*/") == line.npos)
			return line;
		else {
			inMutiComment = false;
			return line + "</div>";
		}
	}

	if (line.length() < 2) {
		return line;
		isPrePage = false;
	}
	if (line[0] != '/') {
		return line;
		isPrePage = false;
	}

	if(line[1]=='/')
		return "<div class=\"comment\"> "+line+"</div>";
	if (line[1] == '*') {
		inMutiComment = true;
		return "<div class=\"comment\">" + line ;
	}
	return line;
	 
}

// -----< private - add generic HTML preliminary markup >-------------------
void CodeConverter::addPreCodeHTML(const std::string& title)
{
  out_ << "<DOCTYPE !HTML>" << std::endl;
  out_ << "<html>" << std::endl;
  out_ << "  <head>" << std::endl;
  out_ << "    <Title>" << title << "</Title>" << std::endl;
  out_ << "    <style>" << std::endl;
  out_ << "      body {" << std::endl;
  out_ << "        padding:15px 40px;" << std::endl;
  out_ << "        font-family: Consolas;" << std::endl;
  out_ << "        font-size: 1.25em;" << std::endl;
  out_ << "        font-weight: normal;" << std::endl;
  out_ << "      }" << std::endl;
  out_ << "      </style>" << std::endl;
  out_ << "  </head>" << std::endl << std::endl;
  out_ << "  <body>" << std::endl;
}

// private addbutton
void CodeConverter::addButton() {
	out_ << "<button onclick = \"toggle('comment')\"> hide / show PreComments </button>" << std::endl;
	out_ << "<button onclick = \"toggle('isFunction')\"> hide / show functions </button>" << std::endl;
	out_ << "<button onclick = \"toggle('isClass')\"> hide / show class </button>" << std::endl;

}
void CodeConverter::addScript() {
	out_ << "<script>" << std::endl;
	out_ << "function toggle(type) {" << std::endl;
	out_ << "var ele = document.getElementsByClassName(type);" << std::endl;

	out_ << "for (i = 0;i < ele.length;i++) {" << std::endl;
	out_ << "var status = ele[i].style.display;" << std::endl;
	out_ << "if (status != 'none' || status == '')" << std::endl;
	out_ << "ele[i].style.display = 'none';" << std::endl;
	out_ << "else" << std::endl;
	out_ << "ele[i].style.display = 'block';" << std::endl;
			//alert(ele[i].style.display);
	out_ << "}" << std::endl;
	out_ << "}" << std::endl;
	out_ << "</script>" << std::endl;
}

// -----< private - add pre tag >------------------------------------------
/* seperated into seperate function to allow for dependencies addition
*  before the actual code of the file */
void CodeConverter::addPreTag()
{
  out_ << "    <pre>" << std::endl;
}

// -----< private - add depedency links markup code >----------------------
void CodeConverter::addDependencyLinks(std::string file)
{
  std::string filename = Path::getName(file);
  if (!dt_.has(file)) { // in case of single file conversion
   // LoggerDebug::write("\n    No entry found in DependencyTable for [" + filename + "]. Skipping dependency links..");
    return;
  }

  if (dt_[file].size() == 0) { // in case the file has no dependencies
   // LoggerDebug::write("\n    No dependencies found for [" + filename + "]. Skipping dependency links..");
    return;
  }

  out_ << "    <h3>Dependencies: " << std::endl;
  for (auto dep : dt_[file]) {
	  if(dt_.table_.find(Path::getFullFileSpec(dep))!=dt_.table_.end())
    out_ << "      <a href=\"" << Path::getName(dep) << ".html\">" << Path::getName(dep) << "</a>" << std::endl;
	  else
		  out_ << " <span>" << Path::getName(dep) << "</span>" << std::endl;

  }
  out_ << "    </h3>";
}

// -----< private - add generic HTML markup closing tags >-----------------
void CodeConverter::addClosingTags()
{
  //out_ << "    </pre>" << std::endl;
  out_ << "  </body>" << std::endl;
  out_ << "</html>" << std::endl;
}

// -----< private - replace HTML special chars >---------------------------
/* note: the function uses while loop to make sure ALL special characters
*  are replaced instead of just the first encounter. */
void CodeConverter::skipSpecialChars(std::string & line)
{
  size_t pos = line.npos;
  while((pos = line.find('<')) != line.npos) 
    line.replace(pos, 1, "&lt;");

  while ((pos = line.find('>')) != line.npos)
    line.replace(pos, 1, "&gt;");
}

vector<std::string> CodeConverter::convertFromDir(std::string dir,std::string reg) {
	Dependency::dependency dep;
	if (createOutpurDir())
		cout << "\n fail to creat output dir";
	//	cout << "\n !!!!!!number of file is " << testFiles->size();
	dep.setpath(dir);
	dep.setregEx(reg);
	dep.analyzeWithPreset();
	setDepTable(*dep.getTable());
     convert();
	return convertedFiles_;
}

#ifdef TEST_CONVERTER

// -----< test stub for converter class >-----------------------------------
int main() {
  //LoggerDebug::attach(&std::cout);
  //LoggerDebug::start();

  //LoggerDebug::title("Testing CodeConverter functions");

  //LoggerDebug::stop(); // do not log DependencyTable logs
  
  DependencyTable dt;
  dt.addDependency("Converter.h", "Converter.cpp");
  dt.addDependency("Converter.cpp", "Converter.h");
  dt.addDependency("A.h", "A.h");
  
  //LoggerDebug::start(); // log from now on

  CodeConverter cc(dt);
  dt.display();
  //LoggerDebug::write("\n");
  cc.outputDir("../convertedFiles"); // will trigger errors which are handled
  cc.convert();

  //LoggerDebug::stop();
  return 0;
}

#endif

#ifdef JieRenTest123
int main() {
	//CodeAnalysis::loader::getpath("../TestFiles", testFiles);
CodeConverter cc;
cc.convertFromDir("../TestFiles","*");

	/*
	for (string file : *testFiles) {
		dep.analyzeFile(file);
		cout << "\ntable size====" << table->table_.size();
		cc.setDepTable(*dep.getTable());
		cc.convertFile(file);
	}
	*/


}
#endif // JieRenTest
