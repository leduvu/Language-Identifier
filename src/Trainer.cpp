////////////////////////////////////////////////////////////////////////////////
// Author:		Le Duyen Sandra Vu 
// Matr.Nr.: 	768693
// Compiler: 	g++ 4.2.1 / clang-700.0.72
// OS:			MacOS 10.11 / Windows 7
// Subject: C++ 1 restricted to use only basic stuff like string and arrays
//
// TRAINER.VERSION 27.04.16
// further description below
////////////////////////////////////////////////////////////////////////////////

#ifndef _TRAINER_CPP_
/// use to check earlier included files
#define _TRAINER_CPP_

#include <iostream>
#include <string>
#include <fstream>
#include "../include/GenerateProfile.hpp"
#include "../include/IdentifyLanguage.hpp"
/*!
 * Trainer.cpp is a program which generates profiles for EVERY corpora (train and test),
 * but it is usually used for training only. The Identifier.cpp will generates 
 * the profiles for the test data.
 *
 * NOTE: GenerateProfile.hpp uses functions from the IdentifyLanguage.hpp
 * that is why it has both included.
 * 
 * INPUT: 1TrainDirectories.txt 2TrainProfDir.txt
 *
 * 1 should contain directories to the test data. 2 will be generatet and will
 * contain the profile directories (also the profs will stored there)
 *
 * \attention Empty lines in the file containing the dirs will not be skipped and causing an exit
 *
 * USAGE: ./program Arg1 Arg2
 *
 * \sa GenerateProfile.hpp
 * \sa IdentifyLanguage.hpp
*/
int main(int argc, char** argv){

	// check arguments
  	if (argc != 3) {
    	std::cerr << "USE: 1TrainDirectories.txt  2TrainProfDir.txt" << std::endl;
    	std::cerr << "1 contains Directories of training data" << std::endl;
    	std::cerr << "2 file which will store the train profile directories afterwards" << std::endl;
    	std::cerr << "(profs will be stored in this directory, too)" << std::endl;
    	exit(1);    
  	}
  	
  	std::string trainFiles 		= argv[1];
  	std::string trainProf 		= argv[2];

	// generate profiles for all trainFiles and testFiles
	// see file GenerateProfile.hpp
	GenerateProfile 	trainData;
	trainData.generate_profile(trainFiles, trainProf);	
	
	return 0;
}

#endif