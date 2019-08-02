#ifndef _Anagrams_hpp_
#define _Anagrams_hpp_

#include <thread>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include <tr1/array>
#include <vector>


namespace AnagramSpace{

//int strComparePlus(std::string a, std::string b);
inline int strComparePlus(std::string a, std::string b){ // Classical was of parameter in C strcmp, but calling to C++ std::compare
    return a.compare(b);
}

//------------------------------------------------------------------------------
template <class T> // return true when a maximum was found, otherwise false.
inline bool assign_max2atomic(std::atomic_int& maximum, const T number)
{
	if( number > maximum ){
		maximum = number;
		return true;
	}else{
		return false;
	}
}


class Anagrams{
    
 public:

	static const unsigned int TOT_Words = 45000;
	static const unsigned int TOT_Family_Words = 100;
	typedef std::tr1::array< std::string, TOT_Family_Words> ArrayFamily; // Array to store the results, chain of anagrams that I call family.

	struct IndexAnagrams{
		std::string Word; // Every word in the file
		std::vector<unsigned int> Anagram_Index; // indexes of the anagrams matching with that word.
		unsigned int IndexesLength;
	};

	struct Dictionary{ // Struct for loading the file of words into a vector
		std::vector<IndexAnagrams> word_struct;
		unsigned int CurrentLength;
	};

    void FindTheLargestFamily(const std::string Filename, const std::string letters);
    // Print all the Anagrams with the same Length or longer than the input.
    
    void AnagramOfGivenLength(const std::string Filename, const std::string letters, const int N);
    // Find the words for a given Length N. (N==0 same Length than the input)

 private:

    static const unsigned int TOT_Characters = 100;
	struct Repetitions{
		std::string Letters; // pattern/word/letters which one we use to filter the list. It will be the first element in the family
		int l; // Length of the Letters string
		int Rep[TOT_Characters]; // number of Repetitions for each character, so we do not have to calculate every time.
	};

	std::atomic_int maxi{1}; // Maximum found. The values of The longer chain of words.


    bool MatchingMethodAll(const std::string input,  const Repetitions pattern);
    bool MatchingMethodN(const std::string input,  const Repetitions pattern, const int N);    
    bool MatchingMethodRange(const std::string input,  const Repetitions pattern, const int N);    
    // Check the words/Anagrams for a given Length N, far All leghts, or just for a range of length.

    void StartConcurrentThread(const int& index, const Dictionary& MotherDic, ArrayFamily Family, int family_len);
    void FollowAnagrams(const int& index, const Dictionary& MotherDic, ArrayFamily& Family, int& familiy_len);
    void PrintList(const Dictionary& list, const int l);
    void FillAnagramsIndexes(Dictionary& list, const int N);
    int ReadDictionary(const std::string Filename, const std::string letters, Dictionary& lists, const int N);
    // ReadDiccionary load the file word list into the RAM memory without the words that don't match with the given letters at first.
    
    Repetitions SetPattern(const std::string letters);
    // Set up a Repetitions object for a given letters/pattern
    
    const bool AnagramCheck(const std::string input, const Repetitions pattern);
    //onst  returns true when input is an Anagram of the given pattern, with the same or more letters.
    
    const int ContainsCharacter( const std::string in, const char c);
    // Return the number or repetitions of char c into string in.
};

}
#endif
