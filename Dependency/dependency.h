#pragma once
//////////////////////////////////////////////////////////////////////////////////////
// dependency.h - a facility to build a dependency table with the help of a parser   //
// ver 1.0                                                                          //
// -----------------------------------------------------------------------          //
// copyright ?Jie Ren, 2019                                                         //
// All rights granted provided that this notice is retained                         //
// -----------------------------------------------------------------------          //
// Language:    Visual C++, Visual Studio 2017                             //
// Platform:    Windows 10                                                   //
// Application: CSE 687 OOD spring2019, project 1                          //
// Author:      Jie Ren , Syracuse University                              //
//              (315) 289-0047, jren21@syr.edu                              //
/////////////////////////////////////////////////////////////////////////////
/*
maintainance history:
	03/06/2019 first release

Module operation: 
	This package is to analysis all the  files with in a given vector<string path> and build 
	a dependency table with it. The dependecny is base on the #include statement in the files.

	This module is assuming that the files is using valid c++ syntax

public interface:
	dependency myanalyser();    /constructor
    
	myanalyser.setpath(vector<string filepath>* paths);
	myanalyser.analyze();
	DependencyTable table=myanalyser.getTable();

	
*/

#include "../DependencyTable/DependencyTable.h"
#include"../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
namespace Dependency {
	class dependency {
		private :
			DependencyTable* table=new DependencyTable();  // this table is modified table which contains a unorderemap<Path,vector<ASTNode*>> moreinformationMap
			CodeAnalysis::Parser* parser;
			std::string path;
			std::string regEx="*";
			CodeAnalysis::Repository* repo;

	public:
		  
		void setpath(std::string);
		void setregEx(std::string);
		void analyzeWithPreset();
		void analyzeFile(std::string);
		DependencyTable* getTable();
		


		


		  

	};
}