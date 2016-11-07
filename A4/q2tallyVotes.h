#include <iostream>
#include <ostream>
#include <iomanip>
#include <vector>
#include "MPRNG.h"

using namespace std;

_Cormonitor Printer;

#if defined( IMPLTYPE_MC )            // mutex/condition solution
// includes for this kind of vote-tallier
class TallyVotes {
    // private declarations for MC
    vector<uint> voted;
    uOwnerLock lock;
    uCondLock voters, bargers;
    bool signaling;


#elif defined( IMPLTYPE_SEM )         // semaphore solution
// includes for this kind of vote-tallier
class TallyVotes {
    // private declarations for this kind of vote-tallier
#elif defined( IMPLTYPE_BAR )         // barrier solution
// includes for this kind of vote-tallier
_Cormonitor TallyVotes : public uBarrier {
    // private declarations for this kind of vote-tallier
    void main();
#else
    #error unsupported voter type
#endif
    // common declarations
    uint groupsize;
    Printer &printer;
    uint currentGroup;
    vector<int> result;
  public:                             // common interface
    TallyVotes( unsigned int group, Printer &printer )
      :
      #if defined( IMPLTYPE_MC )
       signaling(false),
      #elif defined( IMPLTYPE_SEM )
      #elif defined( IMPLTYPE_BAR )
      #endif
      groupsize(group)
      , printer(printer)
      , currentGroup(0)

      {
          result.push_back(0);
          #if defined( IMPLTYPE_MC )
          voted.push_back(0);
      };
    enum Tour { Picture, Statue };
    Tour vote( unsigned int id, Tour ballot );
};

_Task Voter {
    uint id;
    TallyVotes &tallier;
    Printer &printer;
    void main();
  public:
    enum States { Start = 'S', Vote = 'V', Block = 'B', Unblock = 'U', Barging = 'b',
                  Complete = 'C', Finished = 'F' };
    Voter( unsigned int id, TallyVotes &voteTallier, Printer &printer )
      : id(id)
      , tallier(voteTallier)
      , printer(printer)
      {};
};

_Cormonitor Printer {      // chose one of the two kinds of type constructor
    void main();
    void printAndFlush();
    struct col {
        char state = 'N';
        TallyVotes::Tour vote;
        uint numBlocked;
    };
    uint voters;
    col *data;
  public:
    Printer( unsigned int voters ) : voters(voters)
    {
        data = new col[voters];
    };
    ~Printer() {
        cout << "=================" << endl << "All tours started" << endl;
    };
    void print( unsigned int id, Voter::States state );
    void print( unsigned int id, Voter::States state, TallyVotes::Tour vote );
    void print( unsigned int id, Voter::States state, unsigned int numBlocked );
};
