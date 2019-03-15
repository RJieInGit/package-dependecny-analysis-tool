#pragma once
//////////////////////////////////////////////////////////////////////////////////////
// dependency.cpp - a facility to build a dependency table with the help of a parser   //
// ver 1.0                                                                          //
// -----------------------------------------------------------------------          //
// copyright ?Jie Ren, 2019                                                         //
// All rights granted provided that this notice is retained                         //
// -----------------------------------------------------------------------          //
// Language:    Visual C++, Visual Studio 2017                             //
// Platform:    Windows 10                                                   //
// Application: CSE 687 OOD spring2019, project 2                          //
// Author:      Jie Ren , Syracuse University                              //
//              (315) 289-0047, jren21@syr.edu                              //
/////////////////////////////////////////////////////////////////////////////

#include "dependency.h"
#include "../Parser/ConfigureParser.h"
#include <vector>
#include "../loader/loader.h"
using namespace Dependency;
using namespace std;
using namespace CodeAnalysis;

void dependency::setpath(string p) {
	path = p;
}

void dependency::setregEx(string reg) {
	regEx = reg;
}

DependencyTable* dependency::getTable() {
	return table;
}

// analyze with preset. loader the filepaths from load, parse the files
// And during paring, build a dependency table which holds dependecy imformation and ASTree for each file
void dependency::analyzeWithPreset() {

	CodeAnalysis::loader::setregEx(regEx);
	vector<string>* filepaths = new vector < string >;
	loader::getpath(path, filepaths);
	cout << "\n number of files to analyze =" << filepaths->size();
	for (string path123 : *filepaths) {
		string name = FileSystem::Path::getName(path123);

		analyzeFile(path123);          //analyze Each file with seted parser


	}
}


//analyze single file

void dependency::analyzeFile(string path) {
	cout << "/n analyzing file " << path;
	string name = FileSystem::Path::getName(path);
	CodeAnalysis::ConfigParseForCodeAnal configure;
	parser = configure.Build();
	repo = Repository::getInstance();
	table = repo->getDependencyTable();
	try
	{
		if (parser)
		{
			if (!configure.Attach(path))
			{
				std::cout << "\n  could not open file " << name << std::endl;
			}	 //analyze Each file 
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return;
		}		
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		std::cout << "\n  exception caught at line " << __LINE__ << " ";
		std::cout << "\n  in package \"" << name << "\"";
	}
	while (parser->next())
		parser->parse();     // final AST operations
	ASTNode* pGlobalScope = repo->getGlobalScope();
	table->addASTNode(path, pGlobalScope);
	std::cout << "Now parseing __________________________" << pGlobalScope->package_ << "\n"<<"\n\n\n number of staements \n\n\n=" << pGlobalScope->statements_.size();
	for (auto state : pGlobalScope->statements_) {
		if (!state->contains("#"))
			continue;
		string sta = state->ToString();
		int startchar = -1;
		int endchar = -1;
		for (int i = 0;i < sta.length();i++) {
			if (startchar==-1&&(sta[i] == '\"' || sta[i] == '<'))
				startchar = i;
			if (startchar != -1 && (sta[i] == '\"' || sta[i] == '>'))
				endchar = i;
		} 
		if (startchar >= 0 && endchar < sta.length() && startchar < endchar)                            //add dependency  to the table here
			table->addDependency(path,sta.substr(startchar + 1, endchar - startchar - 1));       cout << "\n adding, The size of the talbe is " << table->table_.size();
	}
}
#ifdef TestDependency123
int main() {
	 
	dependency analyzer;
	analyzer.setpath("../TestFiles");
	//cout << "1\n";
	analyzer.analyzeWithPreset();
	//cout << "2\n";
	DependencyTable* dtable = analyzer.getTable();
	dtable->display();
	//cout << "\n3\n";
	for (auto entry:dtable->ASTNodeList) {
		TreeWalk(entry.second);
	}
	int a;
	std::cin >> a;
	return 1;
  }
 
#endif // TestDependency


