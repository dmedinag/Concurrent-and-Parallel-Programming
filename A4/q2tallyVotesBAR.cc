#include "q2tallyVotes.cc"
#include <uCobegin.h>

TallyVotes::Tour TallyVotes::vote( unsigned int id, TallyVotes::Tour ballot ) {


    // register vote
    result += ballot == TallyVotes::Tour::Picture ? +1 : -1;
    // print vote
    printer.print( id, Voter::States::Vote, ballot );
    // wait for the rest
        // if blocking, print blocking
    if( ++voted < groupsize ) {
        printer.print( id, Voter::States::Block, voted );
        uBarrier.block();
        voted--;
        printer.print( id, Voter::States::Unblock, (unsigned int) voted );
    }
    else {
        --voted;
        printer.print( id, Voter::States::Complete );
        uBarrier.block();
    }

    return result[currentGroup-1] > 0 ? TallyVotes::Tour::Picture : TallyVotes::Tour::Statue;
}

void uMain::main() {
    L1: {
        uint v, g, seed;
        if( !checkInput( argv, argc, g, v, seed ) ) break L1;

        myrand.seed(seed);

        Printer p( v );
        TallyVotes tb( g, p );

        COFOR( i, 0, v, Voter a( i, tb, p ); );
    }
}
