////////////////////////////////////////////////////////////////////////////////
// Author:		Le Duyen Sandra Vu 
// Matr.Nr.: 	768693
// Compiler: 	g++ 4.2.1 / clang-700.0.72
// OS:			MacOS 10.11 / Windows 7
// Subject: C++ 1 restricted to use only basic stuff like string and arrays
//
// IDENTIFIER.VERSION 27.04.16
// further description below
////////////////////////////////////////////////////////////////////////////////

#ifndef _TESTER_CPP_
/// use to check earlier included files
#define _TESTER_CPP_

#include <iostream>
#include <string>
#include <fstream>
#include "../include/GenerateProfile.hpp"
#include "../include/IdentifyLanguage.hpp"
/*!
 * Identifier.cpp is a program which identifies languages of text data.
 * 
 * INPUT: TestDirectories.txt TrainProfDir.txt TestProfDir.txt
 *
 * USAGE: ./program Arg1 Arg2 Arg3
 *
 * 1 should contain the directories to the test data. 2 should contain directories to
 * the profiles. 3 will be generatet and will contain the profile directories 
 * (also the profs will stored there)
 *
 * \attention Empty lines in the file containing the dirs will not be skipped and causing an exit
 *
 * \sa GenerateProfile.hpp
 * \sa IdentifyLanguage.hpp
*/
int main(int argc, char** argv){

	// check arguments
  	if (argc != 4) {
    	std::cerr << "USE: 1TestDirectories.txt 2TrainProfDir.txt 3TestProfDir.txt" << std::endl;
    	std::cerr << "1 contains Directories of testing data" << std::endl;
    	std::cerr << "2 contains Directories of training data profiles" << std::endl;
    	std::cerr << "3 file which will store the test profile directories" << std::endl;
    	std::cerr << "(profs will be stored in this directory too)" << std::endl;
    	exit(1);    
  	}
  	
  	std::string testFiles 		= argv[1];
  	std::string trainProf	 	= argv[2];
  	std::string testProf	 	= argv[3];

	// generate profiles for all trainFiles and testFiles
	// see file GenerateProfile.hpp
	GenerateProfile 	testData;
	testData.generate_profile(testFiles, testProf);
	
	// identify the language of all testFiles
	IdentifyLanguage	data;
	data.which_lang(trainProf, testProf); 
	// please use print_withDist() if you want to print with distance
	data.print_withDist();
	
	return 0;
}

#endif