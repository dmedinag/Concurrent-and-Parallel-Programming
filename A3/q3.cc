#include <iostream>
#include <ostream>

#include "MPRNG.h"

#define CONSUMERS 5
#define PRODUCERS 3
#define PRODUCE 10
#define BUFFER_SIZE 10
#ifdef DEBUGOUTPUT
    #define ERROROUTPUT
#endif
#define MAX_INT 2147483647

using namespace std;

/*  The following code was obtained from a topic posted in StackOverflow
*   It is solely used for colouring the console when debugging.
*   credit (last check 10-22-2016):
*   http://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
*/
namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_LGRAY    = 37,
        FG_YELLOW   = 33,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_LGRAY    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            if( mod.code != 39 ) return os << "\033[" << mod.code << "m\033[1m";
            else return os << "\033[" << mod.code << "m\033[0m";
        }
    };
}

// size_t prods = PRODUCERS;
using namespace Color;

Modifier red    (Color::FG_RED);
Modifier lgrey  (Color::FG_LGRAY);
Modifier blue   (Color::FG_BLUE);
Modifier yellow (Color::FG_YELLOW);
Modifier white  (Color::FG_DEFAULT);

_Event E {};

template<typename T> class BoundedBuffer {
  public:
    BoundedBuffer( const unsigned int size )
      :
      front(0), back(0), items(0), size(size), count(0), lock()
      #ifdef NOBUSY
      ,noItems(), noRoom(), wantIn(false)
      #endif
    {
        buffer = ( T* ) malloc( size * sizeof( T ) );
        if( buffer == nullptr ) {
            cout << red << "Error allocating buffer. Stop." << white << endl;
        }
    }

    void insert( T elem ) {
        if( elem == SENTINEL && ++count < prods ) return;

        lock.acquire();
        try {
            while( items >= size ) { noRoom.wait( lock ); }

            buffer[ back++ % size ] = elem;
            back = back % size;
            items++;
            noItems.signal();

        } _Finally { if( lock.owner() == &uThisTask() ) lock.release(); }
    }
    T remove() {

        int res;
        lock.acquire();
        try {
            while( items <= 0 ) { noItems.wait( lock ); }
            // When producers finished, return SENTINEL
            res = buffer[ front++ % size ];
            front = front % size;
            items--;
            noRoom.signal();
        } _Finally { if( lock.owner() == &uThisTask() ) lock.release(); }

        return res;
    }
    ~BoundedBuffer() { free( buffer ); }
  private:
    T *buffer;
    size_t front, back, items, size, count;
    uOwnerLock lock;
    uCondLock noItems, noRoom;
};

_Task Producer {
  public:
    Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay )
      :
        buffer( buffer ),
        Produce( Produce ),
        Delay( Delay )
    {}
    ~Producer() {}

  protected:
    BoundedBuffer<int> &buffer;
    size_t Produce;
    size_t Delay;

    void main() {
        size_t i;
        MPRNG random( getpid() );
        for ( i = 1; i <= Produce; i++ ) {
            // yield form 0 to Delay - 1 times
            yield( random() % ( Delay ) );
            // produce corresponding item
            buffer.insert( (int) i );
        }
        buffer.insert( SENTINEL );
    }

};

_Task Consumer {
  public:
    Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum )
      :
        buffer( buffer ),
        Delay( Delay ),
        Sentinel( Sentinel ),
        sum( sum )
    {}
    ~Consumer() {}

  protected:

    BoundedBuffer<int> &buffer;
    size_t Delay;
    const int Sentinel;
    int &sum;

    void main() {
        sum = 0;
        MPRNG random( getpid() );
        for ( ;; ) {
            // yield form 0 to Delay-1 times
            yield( random() % (Delay) );
            // produce corresponding item
            int value = buffer.remove();
            if( value != Sentinel ) { sum += value; }
            else { break; }
        }
    }
};

bool isInvalid( char *value, string name ) {
    float val = atof(value);
    if( val < MAX_INT && val > 0 ) { return false; }
    return true;
}

void uMain::main () {

    INIT: {

        size_t cons     = CONSUMERS;
        size_t prods    = PRODUCERS;
        size_t produce  = PRODUCE;
        size_t bufsize  = BUFFER_SIZE;
        size_t delay;
        bool badinput = false;

        switch ( argc ) {
            case 1: break;
            case 6: {
                if( isInvalid( argv[5], "Delays") ) {badinput = true;break;}
                delay = atoi( argv[5] );
            }
            case 5: {
                if( isInvalid( argv[4], "BufferSize") ) {badinput = true;break;}
                bufsize = atoi( argv[4] );
            }
            case 4: {
                if( isInvalid( argv[3], "Produce") ) {badinput = true;break;}
                produce = atoi( argv[3] );
            }
            case 3: {
                if( isInvalid( argv[2], "Prods" ) ) {badinput = true;break;}
                prods = atoi( argv[2] );
            }
            case 2: {
                if( isInvalid( argv[1], "Cons" ) ) {badinput = true;break;}
                cons = atoi( argv[1] );
            }
            break;
            default: { badinput = true; }
        }

        delay = prods + cons;
        if( delay < 0 ) { badinput = true; }

        if( badinput ) {
            cout << "Usage: " << argv[0] << " [ Cons (> 0) [ Prods ";
            cout << "(> 0) [ Produce (> 0) [ BufferSize (> 0) [ ";
            cout << "Delay (> 0) ] ] ] ] ]" << endl;
            break INIT;
        }

        // INITIALIZE ENVIRONMENT

        BoundedBuffer<int> buffer( bufsize );
        Producer *producers[ prods ];
        Consumer *consumers[ cons ];
        int sum[ cons ];
        size_t i;

        cout << cons << " " << prods << " " << produce << " " << bufsize << " " << delay << endl;

        // LAUNCH TASKS
        #ifdef __U_MULTI__
        uProcessor p[3] __attribute__ (( unused )); // create 3 kernel thread for a total of 4
        #endif

            // launch producers
        size_t procs = prods > cons ? prods : cons;
        for( i = 0; i < procs; i++ ) {
            if( i < prods ) producers[ i ] = new Producer( buffer, produce, delay );
            if( i < cons ) consumers[ i ] = new Consumer( buffer, delay, SENTINEL, sum[i] );
        }

            // wait for finalizing
        for( i = 0; i < prods; i++ ) { delete producers[ i ]; }
        for( i = 0; i < cons; i++ )  {
            buffer.insert( SENTINEL );
            delete consumers[ i ];
         }

        int total = 0;
        for( i = 0; i < cons; i++ ) total += sum[ i ];
        cout << "total: " << total << endl;
    }
}
