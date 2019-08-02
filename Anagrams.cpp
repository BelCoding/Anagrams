#ifndef _Anagrams_cpp_
#define _Anagrams_cpp_

#include "Anagrams.hpp"
using namespace std;
	
namespace AnagramSpace{

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------
void Anagrams::AnagramOfGivenLength(const std::string Filename, const std::string letters, const int N){
    
	Dictionary CurrentDic;

    if (N >= 0){

        int l = ReadDictionary(Filename, letters, CurrentDic, N);
        PrintList( CurrentDic ,  l);
        cout << "Total anagrams in the CurrentDic (that have " << N << " characters more): "<< l << endl;

    }else{

        cout << "Input length should be >= 0. You said "<< N << endl;

    }
}


void Anagrams::FindTheLargestFamily(const std::string Filename, const std::string letters){

	Dictionary MotherDic;

	ArrayFamily Family;
    Family[0] = letters;
    Repetitions pattern = SetPattern(letters);

    int l = ReadDictionary(Filename, letters, MotherDic, -1);
//    PrintList(MotherDic,  l);

    cout << "Calculating the anagrams in the Dic..."<< endl;
    FillAnagramsIndexes(MotherDic, 1);
    cout << "Building families of words..."<< endl;

		const unsigned int tot_thr = 10;
		std::thread thr[tot_thr];


		unsigned int i = 0; // index of words in MotherDic
		unsigned int t = 0; // counter of threads
    	int len = 1; // length of the temporal family

		while ( i < MotherDic.CurrentLength)
		{

			if ( MatchingMethodN(MotherDic.word_struct[i].Word, pattern, 1) ){


				thr[t%tot_thr] = std::thread(&Anagrams::StartConcurrentThread, this, i, MotherDic, Family, len);
				t++;

				// Wait to join the threads after start tot_thr.
				if(t%tot_thr == 0){ for (unsigned int ii = 0; ii < tot_thr; ++ii) { thr[ii].join(); } }

			}

		++i;
		}

		// Wait to join the remaining threads
		for (unsigned int i = 0; i < t%tot_thr; ++i) { thr[i].join(); }

    cout << "Total anagrams in the Dic: "<< l << endl;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Anagrams::StartConcurrentThread(const int& index, const Dictionary& MotherDic, ArrayFamily Family, int family_len){

	FollowAnagrams(index, MotherDic, Family, family_len);

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Anagrams::FollowAnagrams(const int& index, const Dictionary& MotherDic, ArrayFamily& Family, int& family_len){

if(family_len < static_cast<int>(TOT_Family_Words)){

	Family[family_len] = MotherDic.word_struct[index].Word;
	family_len++;

	const int prev_family_len = family_len;

    if ( assign_max2atomic( maxi, family_len) ){

    	cout << "New maximum found with " << family_len << " elements:" << endl;
    	int i = 0;
		while ( i < family_len)
		{

	    	cout << Family[i] << " ";

		++i;
		}
    	cout << endl << endl;

    }

	if(MotherDic.word_struct[index].IndexesLength > 0){

		unsigned int i = 0;
		while ( i < MotherDic.word_struct[index].IndexesLength)
		{

			FollowAnagrams(MotherDic.word_struct[index].Anagram_Index[i], MotherDic, Family, family_len);
			family_len = prev_family_len;

		++i;
		}

	}else{

	    family_len=0;
	    // end of recursion
	}

}

}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Anagrams::FillAnagramsIndexes(Dictionary& list, const int N){


	unsigned int i=0;
    while ( i < list.CurrentLength)
    {

        Repetitions pattern = SetPattern(list.word_struct[i].Word);
		unsigned int l = 0;
		while ( l < list.CurrentLength)
		{
			// bool Pass = MatchingMethodN(list.word_struct[l].Word, pattern, N);
			// This function control to load only the words N > than pattern length
			if ( MatchingMethodN(list.word_struct[l].Word, pattern, N) && list.word_struct[i].IndexesLength < TOT_Words ){

				list.word_struct[i].Anagram_Index.push_back(l);
				list.word_struct[i].IndexesLength++;

			}
		++l;
		}

    ++i;
    }

}
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Strings Chars, Private methods:

    bool Anagrams::MatchingMethodAll(const std::string input, const Repetitions pattern){
    
    return AnagramCheck( input, pattern) && static_cast<int>(input.length()) >= pattern.l + 1;
    }
    //------------------------------------------------------------------------------

    bool Anagrams::MatchingMethodRange(const std::string input, const Repetitions pattern, const int N){
        
        // Discards the words that are more than N chars longer, and contains all the letters:
        if( AnagramCheck( input, pattern) && static_cast<int>(input.length()) <= pattern.l + N ){ return true; }

    
     return false;
    }
    //------------------------------------------------------------------------------

    bool Anagrams::MatchingMethodN(const std::string input, const Repetitions pattern, const int N){
        
        // Pass only the words that are N (N could be 0) chars longer, and contains all the letters:
        if( AnagramCheck( input, pattern) && static_cast<int>(input.length()) == pattern.l + N ){ return true; }
    
    return false;
    }

    
    //------------------------------------------------------------------------------
    int Anagrams::ReadDictionary(const std::string Filename, const std::string letters, Dictionary& list, const int N){

    std::string in;	
    unsigned int l = 0;
    list.word_struct.clear();
    list.CurrentLength = 0;
    IndexAnagrams empty_word = {};
    empty_word.IndexesLength = 0;

    Repetitions pattern = SetPattern(letters);
    
        ifstream miFichero (Filename);
        if (miFichero.is_open())
        {
            while ( ! miFichero.eof() && l < TOT_Words)
            {
                
            miFichero >> in;

            bool Pass = false;

            	if (N >= 0 && in.length() < TOT_Characters){

					Pass = MatchingMethodN(in, pattern, N); // This function control to load only the words N > than pattern length

				}

            	if(N < 0 && in.length() < TOT_Characters){

					Pass = MatchingMethodAll(in, pattern);    // This function control to load all the words >= than pattern length+1

				}

				if ( Pass ){

                	list.word_struct.push_back(empty_word);
                    list.word_struct[l].Word = in;
                    
                ++l;
                }
                
            }
            miFichero.close();

        } else cout << "The file does not exist/found in the directory " << endl;
    
        if(l == TOT_Words)
            cout << "The CurrentDic was truncated to the first " << TOT_Words << " words." << endl;
        
    list.CurrentLength = l;
    return l;
    }

 //------------------------------------------------------------------------------
    void Anagrams::PrintList(const Dictionary& list, const int l){
            
        string input = "Yes";
        
            if( l > 10 ){

                cout << "The lists.CurrentDic is > 10 words. Contains " << l << " words. Sure that you want to print this? Yes/No" << endl;
                cin >> input;

            }
            
            if ( l > 0 && static_cast<int>(input.length()) > 0 && ( input[0] == 'y' || input[0] == 'Y') ){
                
                int i = 0;
                while ( i < l)
                {
                    cout << "["<< i << "] " << list.word_struct[i].Word;

                   unsigned int ii = 0;
				   while ( ii < list.word_struct[i].IndexesLength)
				   {
					   cout << ", "<< list.word_struct[i].Anagram_Index[ii];
				   ++ii;
				   }
				   cout << endl;
                ++i;
                }   
                
            }
        
    }


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
    Anagrams::Repetitions Anagrams::SetPattern(const std::string letters){
        Repetitions pattern;
        
        pattern.l = static_cast<int>( letters.length());

		if(pattern.l >= static_cast<int>(TOT_Characters))
		{
			cout << "The input pattern has " << pattern.l << " characters that is too long for the system, check out TOT_Characters in AnagramStruct.hpp, or just choose an input file with shorter words."<< endl;
			exit(-1);
		}

        pattern.Letters = letters;

        int c=0;
        while( c < pattern.l ){

            pattern.Rep[c] = ContainsCharacter( letters, letters[c]);

        ++c;
        }

    return pattern;    
    }
    
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

    const bool Anagrams::AnagramCheck( const std::string input, const Repetitions pattern){
    
        int c = 0;
        while( c < pattern.l ){
            
        	// each char in the input should be contained equally or more times than the pattern, to be an Anagram.
            if( pattern.Rep[c] > ContainsCharacter( input, pattern.Letters[c] ) ){ return false; }

        ++c;
        }
        
        // When the word is the same exactly we return false as it is not an Anagram.
        if ( strComparePlus(input, pattern.Letters) == 0){ return false;}
        
    return true;
    }

//------------------------------------------------------------------------------

    	// Returns the number of repetitions for a given character
		const int Anagrams::ContainsCharacter( const std::string in, const char c){ // Return the number or repetitions
		int r=0;

            int i=0;
            while(i < static_cast<int>( in.length() ) ){

            	if(in[i] == c ){ ++r; }

            ++i;
            }

		return r;
		}	

}
#endif
