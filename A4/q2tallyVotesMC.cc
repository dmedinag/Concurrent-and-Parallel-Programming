#include "q2tallyVotes.h"
#include "MPRNG.h"

using namespace std;

bool checkInput( char *argv[], const int argc, uint & g, uint & v, uint & seed ) ;

void Voter::main() {
    cout << "wow voter " << id << " was created" << endl;
   // • yield a random number of times, between 0 and 19 inclusive, so all tasks do not start simultaneously
   // • print start message
   // • yield once using yield( times )
   // • vote (once only)
   // • yield once
   // • print finish message
}

TallyVotes::Tour TallyVotes::vote( unsigned int id, TallyVotes::Tour ballot ) {
    return TallyVotes::Tour::Picture;
}

void TallyVotes::main() {
    
}

void Printer::main() {
    for( uint i = 0; i < voters; ++i ) {
        cout << "Voter" << setw(3) << i;
    }
    cout << endl;
    for( uint i = 0; i < voters; ++i ) {
        cout << setw(8) << "=======";
    }
}

void Printer::print( unsigned int id, Voter::States state ) {
    cout << "printer prints id " << id << " state " << state << endl;
}
void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour vote ) {
    cout << "printer prints id " << id << " state " << state << " vote " << vote << endl;
}
void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ) {
    cout << "printer prints id " << id << " state " << state << " numBlocked " << numBlocked << endl;
}

void uMain::main() {
    L1: {
        uint v, g, seed;
        if( !checkInput( argv, argc, g, v, seed ) ) break L1;

        Printer p( v );
        TallyVotes tb( g, p );
        Voter *voters[v];

        for( size_t i = 0; i < v; ++i ) { voters[i] = new Voter( i, tb, p ); }

        for( size_t i = 0; i < v; ++i ) { delete voters[i]; }
    }

}

bool checkInput( char *argv[], const int argc, uint & g, uint & v, uint & seed ) {
    seed = argc == 4 ?           atoi( argv[3] ) : getpid();
    g = argc == 4 || argc == 3 ? atoi( argv[2] ) : 3;
    v = argc >= 2 || argc <= 4 ? atoi( argv[1] ) : 6;
    if( argc > 4 ) {
        cerr << "Usage: vote [ V [ G [ Seed ] ] ]" << endl;
        return false;
    }
    return true;
}
