////////////////////////////////////////////////////////////////////////////////
// Author:		Le Duyen Sandra Vu 
// Matr.Nr.: 	768693
// Compiler: 	g++ 4.2.1 / clang-700.0.72
// OS:			MacOS 10.11 / Windows 7
// Subject: C++ 1 restricted to use only basic stuff like string and arrays
//
// TESTER.VERSION 27.04.16
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
 * Tester.cpp is a program which tests the language identifying process.
 * 
 * NOTE: It generates the training data profiles new every time the program 
 * will be started. If you want the training and identifying process being seperat
 * please use Trainer.cpp and Identifier.cpp
 * 
 * INPUT: 1TrainDirectories.txt 2TestDirectories.txt 3TrainProfDir.txt 4TestProfDir.txt
 *
 * 1 and 2 should contain directories to the respective data. 3 and 4 will be generatet and will
 * contain the profile directories (also the profs will stored there)
 *
 * \attention Empty lines in the file containing the dirs will not be skipped and causing an exit
 *
 * USAGE: ./program Arg1 Arg2 Arg3 Arg4
 *
 * Generating Time for 20 train languages ca 10kb and 21 test languages ca 4kb
 * on a Mac Book 2012 1GHz and 4GB RAM is 1 minute
 *
 * \sa GenerateProfile.hpp
 * \sa IdentifyLanguage.hpp
*/
int main(int argc, char** argv){

	// check arguments
  	if (argc != 5) {
  		// using std::endl so much to avoid trouble with windows linux \n or \r\n
    	std::cerr << "USE: 1TrainDir.txt  2TestDir.txt  3TrainProfDir.txt  4TestProfDir.txt" << std::endl << std::endl;
    	std::cerr << "1 contains Directories of training data" << std::endl;
    	std::cerr << "2 contains Directories of testing data" << std::endl;
    	std::cerr << "3 file which will store the train profile directories" << std::endl;
    	std::cerr << "(profs will be stored in this directory too)" << std::endl;
    	std::cerr << "4 file which will store the profile directories" << std::endl;
    	std::cerr << "(profs will be stored in this directory, too)" << std::endl;
    	exit(1);    
  	}
  	// save args as strings
  	std::string trainFiles 		= argv[1];
  	std::string testFiles 		= argv[2];
  	std::string trainProf	 	= argv[3];
  	std::string testProf	 	= argv[4];

	// generate profiles for all trainFiles and testFiles
	// see file GenerateProfile.hpp
	GenerateProfile 	trainData;
	trainData.generate_profile(trainFiles, trainProf);
	
	// do not need generating a new object usually , but it might be better
	// if you want to use a getta function
	GenerateProfile 	testData;
	testData.generate_profile(testFiles, testProf);
	
	// identify the language of all testFiles
	IdentifyLanguage	data;
	data.which_lang(trainProf, testProf); 
	data.print_withDist();
	
	return 0;
}

#endif