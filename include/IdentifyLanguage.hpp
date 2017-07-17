////////////////////////////////////////////////////////////////////////////////
// Author:		Le Duyen Sandra Vu 
// Matr.Nr.: 	768693
// Compiler: 	g++ 4.2.1 / clang-700.0.72
// OS:			MacOS 10.11 / Windows 7
// Subject: C++ 1 restricted to use only basic stuff like string and arrays
//
// IDENTIFY LANGUAGE.VERSION 27.04.16
// further description below
////////////////////////////////////////////////////////////////////////////////
 
#ifndef _IDENTIFYLANGUAGE_HPP_
/// use to check earlier included files
#define _IDENTIFYLANGUAGE_HPP_

#include <iostream>
#include <string>
#include <fstream>

/// maximum lines in the generated profile
#define MAX_PROFILE				400	
/// maximum train languages for comparing		
#define MAX_TRAINFILES			40
/// maximum test languages for comparing			
#define MAX_TESTFILES			40			
/**
 * IdentifyLanguage.hpp is a class which calculates the similarity of languages.
 * 
 * print() will print the testing languages and their most similar respective language
 *
 * INPUT: no specific constructor using standard one
 *
 * \pre Generate Profiles first, if not exist.
 * \attention Some wrong generated newlines in the profile can causing a distorted calculation.
*/
class IdentifyLanguage{

private:
	
	/// Will be used to store the results in Store answer[]
	struct Store{
		int			finalDist;
		std::string testFile;
		std::string language;
	};
	std::string language;		// language and dist will be updated in compare
	int dist;					// and initialized in parse_file
	int count_test;				// counter for the exact number of directories
	Store answer[MAX_TESTFILES];// stores the result

/** gets the directories from stream, reads the files, find the closest similarity and 
 * stores them into answer[]
 * \param trainLangRead every line should contain a directory to a train lang profile
 * \param testLangRead  every line should contain a directory to a test lang profile
*/
	bool parse_file(std::istream& trainLangRead, std::istream& testLangRead){
		std::string trainProfName;
		std::string testProfName;
		std::string trainFiles[MAX_TRAINFILES];
		std::string testFiles[MAX_TESTFILES];
		
		// go through every line
		// line should contain fileNames or file directories

		count_test = 0;
		while (testLangRead.good()){	
			// get the file names
			std::getline(testLangRead, testProfName);
			testFiles[count_test] = testProfName;
			count_test++;
		}
		int count_train = 0;
		while (trainLangRead.good()){
			// get the file names
			std::getline(trainLangRead, trainProfName);
			trainFiles[count_train] = trainProfName;
			count_train++;
		}
		
		// compare each test profile with every train profile
		for(int ii = 0; ii < count_test; ii++){
			language = "Calculating problem. No Language found.";	// initialize language
			dist = MAX_PROFILE*MAX_PROFILE;							// initialize dist with max possible
			for(int jj = 0; jj < count_train; jj++){
				if(ii < MAX_TESTFILES && jj < MAX_TRAINFILES){ 
					compare(distance(trainFiles[jj], testFiles[ii]), trainFiles[jj]);
				}
			}
			// store result
			answer[ii].testFile = testFiles[ii];
			answer[ii].language = language;
			answer[ii].finalDist = dist;
		}
		return true;
	} // end of the parse_file function
	
/** gets a profile name for a test and a train language, reads the profiles and
 * calculates the distance
 * \param trainProf profile name of one training language profile
 * \param testProf  profile name of one testing language progile
 * \return distance final calculated distance
*/
	int distance(std::string trainProf, std::string testProf){
		std::ifstream 	trainFileRead;	
		std::ifstream 	testFileRead;		
		std::string		trainGram;
		std::string		testGram;
		std::string	file1[MAX_PROFILE] = {};  // safe the ngrams cause using to while
		std::string file2[MAX_PROFILE] = {};  // loops with stream does not work		
		int distance	= 0;				  // note: it is not the global variable dist

		// check opening process
		trainFileRead.open(trainProf);
		check_open(trainFileRead, trainProf);
		testFileRead.open(testProf);
		check_open(testFileRead, testProf);
		
		// get the content of both files
		
		int	trainIndex 	= 0;
		while(trainFileRead.good() && (trainIndex < MAX_PROFILE)){
			std::getline(trainFileRead, trainGram);
			file1[trainIndex] = trainGram;
			trainIndex++;
		}
		int	testIndex 	= 0;
		while(testFileRead.good() && (testIndex < MAX_PROFILE)){
			std::getline(testFileRead, testGram);
			file2[testIndex] = testGram;
			testIndex++;
		}
		
		// generate the distance and update the global variable distance
		for(int ii = 0; ii < MAX_PROFILE; ii++){
			int found = 0;
			for(int jj = 0; jj < MAX_PROFILE; jj++){
				if(file1[ii] == file2[jj]){
					distance = distance + abs(ii, jj);  // calculate distance
					jj =  MAX_PROFILE;					// if found finish/break this loop		
					found = 1;		
				}
				if((jj == MAX_PROFILE-1) && (found == 0)){
					distance = distance + MAX_PROFILE;  // add the maximum to distance if not found
				}	
			}
		}
		trainFileRead.close();
		testFileRead.close();
		return distance;		
	} // end of the distance function
	
/** checks whether a profile can be opend or not 
 * \param fileStream the stream of a profile
 * \param directory  the directory of the profile
*/
	bool check_open(std::ifstream& fileStream, std::string directory){
		if(!fileStream){
			if(directory != ""){
				std::cerr << "Could not open profile: " << directory << "\n";
				exit(1);
			}
			else{
				std::cerr << "Empty profile or blank. \n";
				exit(1);
			}
		}
		return true;
	} // end of the distance function
	
/** calculates the absolute value of two integers
 * \param index1 minuend
 * \param index2 subtrahend
 * \return absolute value - always positive - no unsigned (mostly recommended is int) 
*/
	int abs(int index1, int index2){
		if(index1 < index2){
			return index2 - index1;	// always return a positive value
		}
		return index1 - index2;
	}
/** gets the calculated distance of two profiles, then update the global value dist
 * and the global value language
 * \param distance generated distance between test and train profile
 * \param langIn   the language name of a train profile
*/
	bool compare(int distance, std::string langIn){
		if(distance < dist){
			language = langIn;		// set language if a smaler distance is found
			dist = distance;		// and set dist to the new value
		}
		return true;
	} // end of the compare function

public:
/** reads the directories, opens the files and passes the content stream to parse_file()
 * \param trainLangProfs file with: every line containing a directory to a train profile
 * \param testLangProfs  file with: every line containing a directory to a test profile
*/
	bool which_lang(std::string trainLangProfs, std::string testLangProfs){
		// generate file objects
  		std::ifstream trainFiles;			// short: std::ifstream trainFiles(langFiles);
  		trainFiles.open(trainLangProfs);	// but without .open then
  		
  		std::ifstream testFiles;		
  		testFiles.open(testLangProfs);		
  		
  		// check opening process
  		if (!trainFiles) {
    		std::cerr << "Could not open file: " << trainLangProfs << std::endl;
    		exit(1);
  		}
  		// check opening process
  		if (!testFiles) {
    		std::cerr << "Could not open file: " << testLangProfs << std::endl;
    		exit(1);
  		}
		
		parse_file(trainFiles, testFiles);
		trainFiles.close();
		testFiles.close();
		return true;
	} // end of the which_lang function
	
/// prints the test language directory and the calculated most similar language
	bool print(){
		for(int ii = 0; ii < count_test; ii++){
			std::cout << answer[ii].testFile << "\n" << get_name(answer[ii].language) << "\n\n";
		} // end of the print function	
		return true;
	}
/// prints the test language directory, the calculated most similar language and the smalles distance
	bool print_withDist(){
		for(int ii = 0; ii < count_test; ii++){
			std::cout << answer[ii].testFile << "\n" 
			<< get_name(answer[ii].language) << " "
			<< answer[ii].finalDist << "\n\n";
		} // end of the print function	
		return true;
	}

/** cuts the directory and the ending of the path off and returns the name
 * \param directory a full path of a file
 * \return get_substring(directory, left, right) the name of the file
 * note: the substring for no '.' and '/' will be 0 to directory.length()
 * meaning the name will not change
*/
	std::string get_name(std::string directory){
		int right 			= directory.length();
		int left  			= 0;
		int once1 			= 0;	
		int once2  			= 0;
		
		for(int ii = directory.length(); ii > 0; ii--){
			// the last '.' after //  should be the one marking the ending of the name
			if((directory[ii] == '.') && (once1 == 0) && (once2 == 0)){
				right = ii-1;
				once1 = 1;
			}
			// the last '/' should be the one marking the beginning of the name
			if(((directory[ii] == '\\') || (directory[ii] == '/')) && (once2 == 0)){
				left  = ii+1;
				once2 = 1;
			}
		}
		return get_substring(directory, left, right);
	} // end of the get_name function

/** cuts the name and the ending off a path off and returns the directory
 * \param directory a full path of a file
 * \return get_substring(directory, left, ii) the directory of the file
 * \return directory the string if no directory is found
*/
	std::string get_directory(std::string directory){
		int right 			= directory.length();
		int left  			= 0;
		
		// delete everything after seeing the last '/'	
		for(int ii = directory.length(); ii > 0; ii--){
			if(((directory[ii] == '\\') || (directory[ii] == '/'))){
				return get_substring(directory, left, ii);
			}
		}
		return directory;	
	}
/** cuts everything off before and after given boundaries of a string
 * \param string the original string
 * \param left   the left most boundary
 * \param right  the right most boundary
 * \return substring a substring of the original one
*/
	std::string get_substring(std::string string, int left, int right){
		std::string substring = "";
		for(int ii = left; ii <= right; ii++){
			substring = substring + string[ii];
		}
		return substring;
	} // end of the get_substring function
	
}; // end of the Compare class

#endif