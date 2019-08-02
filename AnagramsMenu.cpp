#include "Anagrams.cpp"

using namespace AnagramSpace;


	void PrintHelp(){
    cout << "Anagrams..." << endl;

    cout << "We are looking for derived anagrams in a dictionary. Derived anagram is a word that consists of all the letters from the base word plus one more letter, rearranged ex. sail → nails → aliens . Write a code in C/C++ (working in terminal) that will import dictionary from external text file (one word each row)." << endl; 
    cout << "Than one should be able to input a starting 3 letter word and search for the longest anagram chain based on this starting word and the dictionary. All words in the chain must be contained in the dictionary. There may be more than one longest chain starting from a given word and optionally you may find a way to show them alternatively." << endl;
    cout << "We are expecting documentation of used algorithm either in the code or in a separate text file or both." << endl;
    cout << endl;
    cout << "I downloaded the word lists for the programm data base: Data/vocab_100k.txt  Data/wlist_match12.txt  Data/wlist_match1.txt"<< endl;
    
    cout << "Data source:"<< endl;
    cout << "https://www.keithv.com/software/wlist/"<< endl;
    cout << endl;    
    cout << "--Help"<< endl;
    cout << "-Option Path/DictionaryFile Chars"<< endl << endl;
    
    cout << "-All Data/wlist_match1.txt hola"<< endl;
    cout << "Print the list of All anagrams for the given file and letters."<< endl << endl;
    
    cout << "-Anagrams Data/wlist_match1.txt hola 0"<< endl;
    cout << "Display all anagrams for a given Length. 0 -> Anagrams with the same Length than hola [4]." << endl;
    cout << "1 -> Anagrams one char longer [5] than hola [4]..." << endl;
    cout << "-1 -> Not possible by definition." << endl << endl;
        
    }		

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

int main(int argc, char* argv[]){

    //system("date");
    Anagrams An;
    bool pass=false;
    
    int i=1;
    while(i < argc){
		
        if( ( strComparePlus(argv[i], "--Help")==0  ||  strComparePlus(argv[i], "-Help")==0  ||  strComparePlus(argv[i], "--help")==0  ||  strComparePlus(argv[i], "-help")==0  ||  strComparePlus(argv[i], "-h")==0  || strComparePlus(argv[i], "-H")==0 ||  strComparePlus(argv[i], "--H")==0 ||  strComparePlus(argv[i], "--h")==0) && argc > i){	
            PrintHelp();
            pass=true;
		}
        if( ( strComparePlus(argv[i], "-All")==0 ) && argc == 4 ){	
            
            const std::string Filename = argv[i+1];
            const std::string letters = argv[i+2];
            system("date");
            An.FindTheLargestFamily(Filename, letters);
            system("date");

            pass=true;
        }

        
        if( ( strComparePlus(argv[i], "-Anagrams")==0 ) && argc == 5 ){	
            
            const std::string Filename = argv[i+1];
            const std::string letters = argv[i+2];
            const int N = atoi(argv[i+3]);
            
            An.AnagramOfGivenLength(Filename, letters, N);
            pass=true;
        }	


    i++;
	}
	
	if(!pass)
    {
        cout << "If you need help Type: ./AnagramsMenu --Help" << endl;
        cout << "Parameters given: " << argc-1 << endl;
    }
    //system("date");
}
