#include "q3tallyVotes.cc"
#include <uCobegin.h>

TallyVotes::Tour TallyVotes::vote( unsigned int id, TallyVotes::Tour ballot ) {

    // register vote
    result += ballot == TallyVotes::Tour::Picture ? +1 : -1;

    // print vote
    printer.print( id, Voter::States::Vote, ballot );
    // wait until the result is ready
    printer.print( id, Voter::States::Block, ++blocked );
    ++voted;
    lastid = id;
    cond.wait();
    // out! Tell the Printer that we are done waiting, and how many are left to be.
    printer.print( id, Voter::States::Unblock, --blocked );

    return ret > 0 ? TallyVotes::Tour::Picture : TallyVotes::Tour::Statue;
}

void TallyVotes::main() {

    for( ;; ) {
        if( voted == groupSize ) {
            printer.print( lastid, Voter::States::Complete );

            ret = result; // we don't want our result to be altered by the next poll
            result = 0; // nor alter the other poll's result!
            voted = 0;

            // let's unblock our mates
            // signal makes the signalled take the context, don't let them ruin our collaborative work!
            for( uint i = blocked; i > 0 ; --i ) cond.signalBlock();

            // start new poll if any, terminate otherwise.
            _Accept( ~TallyVotes ) { break; }
            or _Accept( vote ) { continue; }
        }
        _Accept( vote );
    }
}

void uMain::main() {
    L1: {
        uint v, g, seed;
        if( !checkInput( argv, argc, g, v, seed ) ) break L1;

        myrand.seed( seed );

        Printer p( v );
        TallyVotes tb( g, p );

        COFOR( i, 0, v, Voter a( i, tb, p ); );
    }
}
