####################################################################################
# Makefile for Mac OSX tested on OSX El Capitan (10.11.4)
#
# Author:	Le Duyen Sandra Vu 
# Matr.Nr.: 	768693
# Compiler: 	g++ 4.2.1
#
# usage: make command
# example: make test
#
# summary
#	compile		compiles everything and copies the executables in the bin
#	test		compiles and starts the train then identify
#	train		compiles and starts the trainer program
#	identify	compiles and starts the identifier program 
#	documentation	goes to doc and generates doxygen html
#			(„, \“ needed to stay in this dir)
#	clean		deletes everything which can be generated
#
# NOTE: if compiled and started from src the data directories in data all need ../
# in front - like: ../data/test_data/deutsch.txt
####################################################################################


compile: src/Trainer.cpp src/Identifier.cpp include/GenerateProfile.hpp include/IdentifyLanguage.hpp
	g++ -O3 -o trainer src/Trainer.cpp
	cp trainer bin/
	g++ -O3 -o identifier src/Identifier.cpp
	cp identifier bin/
	
test: src/Tester.cpp include/GenerateProfile.hpp include/IdentifyLanguage.hpp
	g++ -O3 -o trainer src/Trainer.cpp
	./trainer data/train_dir.txt data/trainP.txt
	g++ -O3 -o identifier src/Identifier.cpp
	./identifier data/test_dir.txt data/trainP.txt data/testP.txt

train: src/Trainer.cpp include/GenerateProfile.hpp include/IdentifyLanguage.hpp
	g++ -O3 -o trainer src/Trainer.cpp
	./trainer data/train_dir.txt data/trainP.txt

identify: src/Identifier.cpp include/IdentifyLanguage.hpp
	g++ -O3 -o identifier src/Identifier.cpp
	./identifier data/test_dir.txt data/trainP.txt data/testP.txt

documentation:
	cd doc; \
	doxygen LanguageIdentify.doxygen

clean:
	rm -rf *.o tester
	rm -rf *.o trainer
	rm -rf *.o identifier 
	rm -rf data/*.prof
	rm -rf data/trainP.txt 
	rm -rf data/testP.txt
	rm -rf doc/html
	rm -rf *.o bin/*