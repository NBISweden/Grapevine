#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"

#include "nl/mgrmstck.h"
#include "nl/mgrmgen.h"
#include "nl/mxtract.h"

void PreProcess(CMString & out, const CMPtrStringList & words, const char * szText)
{
  int i, j;
  CMPtrStringList tokens;
  Tokenize(tokens, szText);

  out = "";
  
  for (i=0; i<tokens.length(); i++) {
    bool b = false;
    for (j=0; j<words.length(); j++) {
      if (*tokens(i) == *words(j)) {
	out += *tokens(i);
	out += " ";       
	b = true;
	break;
      }
    }
    if (!b) {
      for (j=0; j<(int)strlen(*tokens(i)); j++) {
	out += (*tokens(i))[j];
	out += " ";
      }

    }
  }
  cout << "Formatted to " << out << endl;
}

int main( int argc, char** argv )
{

  commandArg<string> fileCmmd("-i","grammar file");
  commandLineParser P(argc,argv);
  P.SetDescription("Loads a grammar and parses input from the command line.");
  P.registerArg(fileCmmd);
 
  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);

  int i, j;

  CMGrammarStack grmStack;

  
  try {
    cout << "Loading grammar: " << fileName << endl;
    GRAMMAR_HANDLE h = grmStack.AddGrammaAndReadFromFile(fileName.c_str());
	    
    cout << "Complete!" << endl;
  }

  catch(CMException & ex) {
    
    cout << "Exception cought during load!!" << endl;
    ex.Print();
    return 0;
  }

  CMPtrStringList words;
  grmStack.GetWordList(words);


  
  char szText[512];
  

  do {
    cout << "> ";
    cin.getline(szText, 512);
    if (strcmp(szText, "exit") == 0)
      break;
    
    CMPtrStringList result;
    CMString input;
    PreProcess(input, words, szText);
    GRAMMAR_HANDLE h = grmStack.ParseAndEvaluate(result, input);
    
    if (h != -1) {
      cout << "Parsed w/ handle " << h << ", length=" << result.length() << endl;
      for (int j=0; j<result.length(); j++) {
	cout << ((const char*)*result(j)) << endl;
      }	
    } else {
      cout << "Nope, didn't parse... " << endl;
    }
    //ParseAndPrint(parser, szText);
  } while (1 == 1);
  



  
   return 0;
}
