////////////////////////////////////////////////////////////////////////////////
// Author:		Le Duyen Sandra Vu 
// Matr.Nr.: 	768693
// Compiler: 	g++ 4.2.1 / clang-700.0.72
// OS:			MacOS 10.11 / Windows 7
// Subject: C++ 1 restricted to use only basic stuff like string and arrays
//
// GENERATE PROFILE.VERSION 27.04.16
// further description below
////////////////////////////////////////////////////////////////////////////////

#ifndef _GENERATEPROFILE_HPP_
/// use to check earlier included files
#define _GENERATEPROFILE_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include "../include/IdentifyLanguage.hpp"	
		// using the get_name/get_substring/get_directory function of this .hpp
		// it's only used in the to_file and sum_toFile function

/// all macro descriptions are in the comments, it is clearer
#define MAX_READ_CHAR       100000			// maximum characters, which should be read
#define MAX_UNIGRAMS		5000			// maximum unigrams of an array of structs
#define MAX_BIGRAMS			5000			// maximum bigrams of an array of structs
#define MAX_TRIGRAMS		5000			// maximum trigrams of an array of structs
#define MAX_NGRAMS			MAX_TRIGRAMS + MAX_BIGRAMS + MAX_UNIGRAMS;  // combination of all
#define MAX_PROFILE			400				// maximum lines in the generated profile
#define MAX_FILES			40				// maximum files to calculate profile
/**
 * GenerateProfile.hpp is a class which generates the profile for text corpora.
 * 
 * INPUT: no specific constructor using standard one
 *
 * \bug There are some characters which could not be filter out and causing
 * a new line. So it will bring some calculation failures in the IdentifyLanguage.hpp and more lines
 * in the profile then it should be.
*/
class GenerateProfile{

private:
	/// struct with char for speed - no convert to string yet
	/// for storing unigrams
	struct UniFrequence{
		char ngram;
		int  freq;
	};
	/// struct for storing all sort of ngrams
	struct MultiFrequence{
		std::string ngram;
		int			freq;
	};
	
	// global Variables
	UniFrequence 	uni_grams[MAX_UNIGRAMS];		// containing all unigrams (unique)
	MultiFrequence	bi_grams[MAX_BIGRAMS];			// containing all bigrams (unique)
	MultiFrequence	tri_grams[MAX_TRIGRAMS];		// containing all trigrams (unique)
	MultiFrequence	all_ngrams[MAX_TRIGRAMS + MAX_BIGRAMS + MAX_UNIGRAMS]; // containing all ngrams (unique)
	std::string 	directories[MAX_FILES];			// containing all the content of the file with the dirs
	int uni_tillMax;	// counter to avoid border violation
	int bi_tillMax;		// counter to avoid border violation
	int tri_tillMax;	// counter to avoid border violation
	int ngram_size;		// counter to avoid border violation
	int fileNumber;		// counter to keep track for file quantity

/** gets the directories from the stream and the file path of the file where the dirs 
 * should be stored, reads the files, find the closest similarity and stores them into answer[]
 * \param langFiles file with: every line should contain a directory to a train lang profile
 * \param storeDirectory  the path of the file where the dirs should be stored
*/
	bool parse_file(std::istream& langFiles, std::string storeDirectory){	
		std::string fileName;	
		// go through every line
		// line should contain fileNames or file directories
		while (langFiles.good()){
			// initialize structs
			reset_unigrams();
			reset_bigrams();
			reset_trigrams();
			reset_ngrams();
			
			// get the file names
			std::getline(langFiles, fileName);
			// generate their profile
			generate(fileName, storeDirectory);
			// safe the directories for sum_toFile()
			directories[fileNumber] = fileName;
			fileNumber++;
		}
		return true;
	} // end of the parse_file function

/** gets the test file path and the file path of the file where the dirs should be stored,
 * segmentates (generating ngrams), merges uni, bi and tri grams, sorts them and write them
 * into a file
 * \param fileName test file path
 * \param storeDirectory file path of the file where the dirs should be stored
*/
	bool generate(std::string fileName, std::string storeDirectory){
		segmentate(fileName);
		int right = 0;
		
		merge();							// merges uni, bi and tri gramm struct
		sort(right, ngram_size);			// sort: the common first
		to_file(fileName, storeDirectory);	// write to file .prof
		return true;
	} // end of the generate function	

/** gets a file path, reads the content of the file and stores it in the respective struct
 * \param fileName file path of a text corpus
*/	
	bool segmentate(std::string fileName){
		std::ifstream 	fileRead;
		char			chara;
		int				readChars = 0;
		
		// check opening process
		fileRead.open(fileName);
		if(!fileRead){
			// if could not open file or file is empty, print error
			if(fileName != ""){
				std::cerr << "Could not open the corpus-file: " << fileName << "\n";
				exit(1);
			}
			else{
				std::cerr << "No directory found. Empty file or blank." << fileName << "\n";
				exit(1);
			}
		}
		
		char			firstChar;
		char			secondChar;
		char			thirdChar;
		uni_tillMax		= 0;		// initialize counter
		bi_tillMax 		= 0;		// initialize counter
		tri_tillMax 	= 0;		// initialize counter
		
		while(fileRead.good() && readChars < MAX_READ_CHAR){
			// read 3 characters at once
			fileRead.get(firstChar);
			fileRead.get(secondChar);
			fileRead.get(thirdChar);
			
			// call the chars and store them into the structs
			uni_set(cull(firstChar));
			uni_set(cull(secondChar));
			uni_set(cull(thirdChar));
			
			bi_set(cull(firstChar), cull(secondChar));
			bi_set(cull(secondChar), cull(thirdChar));
		
			tri_set(firstChar, secondChar, thirdChar);
			
			uni_tillMax++;
			bi_tillMax++;
			tri_tillMax++;
			readChars++;
		}
		fileRead.close();
		return true;
	} // end of the segmentate function

/// gets a char and replaces it with a '_' if it is a special symbol
/// \param original a character
	char cull(char original){
		// compare the input with all the symbols of the
		// symbol string - in case of matching replace with ' '
		// else return the original character 
		std::string symbols = ".,[]()-+#^° ?!123456789%&$§<>*/\n";
		for(int jj = 0; jj < symbols.length(); jj++){
			if(original == symbols[jj]){
				return '_';
			}
		}
		return original;
	} // end of the cull function
	
/// gets a char and adds it to the global uni_gram[] struct
/// \param chara a character
	bool uni_set(char chara){
		// look if the character already exists 
		// if no then put it into the struct and set the freq to 1
		// if yes go to the right position and increment the freq
		
		if(uni_tillMax < MAX_UNIGRAMS){
			int found = is_in1(chara);
			if(found == -1){
				uni_grams[uni_tillMax].ngram 	= chara;
				uni_grams[uni_tillMax].freq 	= 1;
				uni_tillMax++;
			}
			else{uni_grams[found].freq++; }
		}
		return true;
	} // end of the uni_set function	

/// gets two chars and adds it to the global bi_gram[] struct
/// \param chara1 a character \param chara2 a character	
	bool bi_set(char chara1, char chara2){
		// see uni_set function
		if(bi_tillMax < MAX_BIGRAMS){
			int found = is_in2(chara1, chara2);
			if(found == -1){
				bi_grams[bi_tillMax].ngram = 	std::string(1, chara1) + 
												std::string(1, chara2);
				bi_grams[bi_tillMax].freq 	= 1;
				bi_tillMax++;
			}
			else{bi_grams[found].freq++;}	
		}
		return true;
	} // end of the bi_set function

/// gets three chars and adds it to the global tri_gram[] struct
/// \param chara1 a character \param chara2 a character	\param chara3 a character	
	bool tri_set(char chara1, char chara2, char chara3){
		// see uni_set function
		if(tri_tillMax < MAX_TRIGRAMS){
			int found = is_in3(chara1, chara2, chara3);
			if(found == -1){
				tri_grams[tri_tillMax].ngram = std::string(1, chara1) + 
												std::string(1, chara2) + 
												std::string(1, chara3);	
				tri_grams[tri_tillMax].freq 	= 1;				
				tri_tillMax++;
			}
			else{tri_grams[found].freq++;}
		}
		return true;
	} // end of the tri_set function
	
/// gets a char and search it in the whole uni_gram[] struct
/// \param chara a character	
	int is_in1(char chara){
		// go through the hole struct and surch for the character
		// in case of matching return the position else -1
		for (int tt = 0; tt < MAX_UNIGRAMS; tt++) {
			if (uni_grams[tt].ngram == chara) 
				return tt;
		}
		return -1;
	} // end of the is_in1 function

/// gets two chars, convert them to a string and search it in the whole bi_gram[] struct
/// \param chara1 a character \param chara2 a character	
	int is_in2(char chara1, char chara2){
		// see is_in1 function
		for (int tt = 0; tt < MAX_BIGRAMS; tt++) {
			if (bi_grams[tt].ngram == (	std::string(1, chara1) + 
										std::string(1, chara2))) 
				return tt;
		}
		return -1;
	} // end of the is_in2 function

/// gets three chars, convert them to a string and search it in the whole tri_gram[] struct
/// \param chara1 a character \param chara2 a character	\param chara3 a character	
	int is_in3(char chara1, char chara2, char chara3){
		// see is_in1 function
		for (int tt = 0; tt < MAX_TRIGRAMS; tt++) {
			if (tri_grams[tt].ngram == (std::string(1, chara1) + 
										std::string(1, chara2) + 
										std::string(1, chara3))) 
				return tt;
		}
		return -1;
	} // end of the is_in3 function
	
/// initialize/reset global uni_grams[] struct
	bool reset_unigrams(){
		for(int kk = 0; kk < MAX_BIGRAMS; kk++){
			uni_grams[kk].ngram = '\0';		// initialize every ngram
			uni_grams[kk].freq 	= 0;		// initialize every freq
		}
		return true;
	} // end of the reset_bigrams function

/// initialize/reset global bi_grams[] struct
	bool reset_bigrams(){
		for(int kk = 0; kk < MAX_BIGRAMS; kk++){
			bi_grams[kk].ngram 	= "";		// initialize every ngram
			bi_grams[kk].freq 	= 0;		// initialize every freq
		}
		return true;
	} // end of the reset_bigrams function	

/// initialize/reset global tri_grams[] struct
	bool reset_trigrams(){
		for(int kk = 0; kk < MAX_TRIGRAMS; kk++){
			tri_grams[kk].ngram = "";		// initialize every ngram
			tri_grams[kk].freq 	= 0;		// initialize every freq
		}
		return true;
	} // end of the reset_trigrams function

/// initialize/reset global all_ngrams[] struct
	bool reset_ngrams(){
		int max_ngrams = MAX_NGRAMS;
		for(int kk = 0; kk < max_ngrams; kk++){
			all_ngrams[kk].ngram 	= "";	// initialize every ngram
			all_ngrams[kk].freq 	= 0;	// initialize every freq
		}
		return true;
	} // end of the reset_ngrams function

/// puts the uni_gram[], bi_gram[] and tri_gram[] struct into the all_ngrams[] struct
	bool merge(){
		// copy all values to the all_ngrams[] struct
		// ngram_size keeps track about the right index assigning of the struct
		ngram_size = 0;
		
		for(int pp = 0; pp < MAX_TRIGRAMS; pp++){
			if(tri_grams[pp].freq >= 1){
				all_ngrams[ngram_size].ngram = tri_grams[pp].ngram;
				all_ngrams[ngram_size].freq  = tri_grams[pp].freq;
				ngram_size++;
			}
		}
		
		for(int pp = 0; pp < MAX_BIGRAMS; pp++){
				if(bi_grams[pp].freq >= 1){
					all_ngrams[ngram_size].ngram = bi_grams[pp].ngram;
					all_ngrams[ngram_size].freq  = bi_grams[pp].freq;
					ngram_size++;
			}
		}		
		
		int max_ngrams = MAX_NGRAMS;
		for(int pp = 0; pp < MAX_UNIGRAMS; pp++){
			if(uni_grams[pp].freq >= 1){
				all_ngrams[ngram_size].ngram = uni_grams[pp].ngram;
				all_ngrams[ngram_size].freq  = uni_grams[pp].freq;	
				ngram_size++;
			}
		}
		return true;
	} // end of the merge function

/** starts with two boundaries of an array - mostly the beginning and end index
 * then using partition to swap a small value with a bigger one, does it recursive
 * till everything is sorted
 * \param left a left boundary \param right a right boundary
*/
	bool sort(int left, int right){
		// termination
		int index;
		if(left < right){
			// using partition
			index = partition(left, right);
			// sort the two subarrays
			sort(left, index-1);	// first half	
			sort(index+1, right);	// second half
			return true;
		}
		return false;	
	} // end of the sort function
	
/** gets two boundaries of an array, pivot will be the value with the index of
 * the left boundary, then searching for a bigger value starting from the left of the array
 * and searching for a smaller value starting from the right of the array, then swap them
 * \param left a left of an array boundary \param right a right boundary of an array
*/
	int partition(int left, int right){
		int tempLeft = left;
		int tempRight = right;
		int pivot = all_ngrams[left].freq; 
		
		while(tempLeft < tempRight){
			// search for an Element on the right side, 
			// which is smaller than pivot (or same)
			while((all_ngrams[tempRight].freq <= pivot)){
				tempRight--;
			}
			// search for an Element on the left side, 
			// which is larger than pivot			
			while((all_ngrams[tempLeft].freq > pivot)){
				tempLeft++;
			}
			if(tempLeft < tempRight){
				// swap them
				swap(tempRight, tempLeft);
			}
		}
		return tempLeft;		
	} // end of the partition function

/// gets two indices of an array, then swaps the values at that position
/// \param left an index of an array \param right an index of an array
	bool swap(int left, int right){
		// save Value
		int tempFreq 			= all_ngrams[left].freq;
		std::string tempGramm 	= all_ngrams[left].ngram;
		
		// assign the value of the other element
		all_ngrams[left].freq 	= all_ngrams[right].freq;
		all_ngrams[right].freq	= tempFreq;
		
		// assign the saved value
		all_ngrams[left].ngram 	= all_ngrams[right].ngram;
		all_ngrams[right].ngram	= tempGramm;
		return true;
	} // end of the swap function

/** gets the directories from stream and cut off the directories = name,
 * gets the the path of the file where the dirs should be stored and cut off the names = dir,
 * merge directory with names + '.prof' and use this as the filename of the profiles,
 * then write the ngrams to the profile
 * \param fileName file with: every line should contain a directory to a train lang profile
 * \param storeDirectory the path of the file where the dirs should be stored
*/	
	bool to_file(std::string fileName, std::string storeDirectory){
		int max_ngrams = MAX_NGRAMS;
		std::ofstream fileWrite;
		IdentifyLanguage object;
		std::string address = 	object.get_directory(storeDirectory)+ 
								object.get_name(fileName) + ".prof";
		
		// rewrite file, will not append
		fileWrite.open(address);
		// check opening process
		if(!fileWrite){
			std::cerr << "Could not open profile: " << address + ".prof" << std::endl;
			exit(1);
		}
		// write n-gramms to file with the limit of lines set in MAX_PROFILE
		for(int kk = 0; kk < MAX_PROFILE; kk++){
			if(MAX_PROFILE < max_ngrams){
				// do not put a new line at the end
				if(kk == 0){
					fileWrite << all_ngrams[kk].ngram;
				}
				else{ fileWrite <<  std::endl << all_ngrams[kk].ngram; }	
			}
		}
		fileWrite.close();
		return true;
	} // end of the to_file function

/** gets the directories from directories[] and cut off the directories = name,
 * gets the the path of the file where the dirs should be stored and cut off the names = dir,
 * merge directory with name + '.prof' and write the paths into a file
 * \param storeDirectory the path of the file where the dirs should be stored
*/	
	bool sum_toFile(std::string storeDirectory){
		std::string fileName = "";
		std::ofstream fileWrite;
		fileWrite.open(storeDirectory);
		IdentifyLanguage object;
		
		if(!fileWrite){
			std::cerr << "Could not open file to store directories: " << storeDirectory << std::endl;
			exit(1);
		}
		
		// write to file but do not append a new line at the end
		for(int ii = 0; ii < fileNumber; ii++){			
			if(ii == 0){
				fileWrite << 	object.get_directory(storeDirectory) +  
								object.get_name(directories[ii]) + ".prof";
			}
			else{ 
				fileWrite << std::endl << object.get_directory(storeDirectory) +  
								object.get_name(directories[ii]) + ".prof";
			}
		}
		fileWrite.close();
		reset_directories();
		return true;
	}

/** clears the global array directories[] 
 * \attention should not be used on an empty array (if compile with -O3 segfault),
 * check with sizeof did not work
*/
	bool reset_directories(){
		for(int ii = 0; ii < fileNumber; ii++){
			directories[ii].clear();
		}
		return true;
	}	
	
public:
/** reads the directories, opens the files and passes the content stream to parse_file()
 * \param langFiles file with: every line containing a directory to a text corpus
 * \param storeDorectory the path of the file where the dirs should be stored
*/
	bool generate_profile(std::string langFiles, std::string storeDirectory){
		fileNumber = 0;
		// generate file object
  		std::ifstream trainFiles;		// short: std::ifstream trainFiles(langFiles);
  		trainFiles.open(langFiles);		// but without .open then
  		// check opening process
  		if (!trainFiles) {
    		std::cerr << "Could not open/create file: " << langFiles << std::endl;
    		exit(1);
  		}
		parse_file(trainFiles, storeDirectory);
		trainFiles.close();
		
		sum_toFile(storeDirectory);
		return true;
	} // end of the generate_profile function
		
}; // end of the GenerateProfile class

#endif