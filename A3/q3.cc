#include <iostream>
#include <ostream>
#include "q3.h"

#define CONSUMERS 5
#define PRODUCERS 3
#define PRODUCE 10
#define BUFFER_SIZE 10
#define MAX_INT 2147483647

using namespace std;

void Producer::main() {
    int i;
    for ( i = 1; i <= Produce; i++ ) {
        // yield form 0 to Delay - 1 times
        yield( MPRNG( getpid() )(0, Delay-1) );
        // produce corresponding item
        buffer.insert( (int) i );
    }
}

void Consumer::main() {
    sum = 0;
    for ( ;; ) {
        // yield form 0 to Delay-1 times
        yield( MPRNG( getpid() )(0 ,Delay-1) );
        // produce corresponding item
        int value = buffer.remove();
        if( value != Sentinel ) { sum += value; }
        else { break; }
    }
}

bool isInvalid( char *value, string name ) {
    float val = atof(value);
    if( val < MAX_INT && val > 0 ) { return false; }
    return true;
}

void uMain::main () {

    INIT: {

        int cons     = CONSUMERS;
        int prods    = PRODUCERS;
        int produce  = PRODUCE;
        int bufsize  = BUFFER_SIZE;
        int delay;
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

        if( argc < 6) delay = prods + cons;
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
        int i;

        // LAUNCH TASKS
        #ifdef __U_MULTI__
        uProcessor p[3] __attribute__ (( unused )); // create 3 kernel thread for a total of 4
        #endif

            // launch producers
        for( i = 0; i < prods; i++ ) {
            producers[ i ] = new Producer( buffer, produce, delay );
        }
        for( i = 0; i < cons; i++ ) {
            consumers[ i ] = new Consumer( buffer, delay, SENTINEL, sum[i] );
        }

            // wait for finalizing
        for( i = 0; i < prods; i++ ) { delete producers[ i ]; }
        for( i = 0; i < cons; i++ ) { buffer.insert( SENTINEL ); }
        for( i = 0; i < cons; i++ ) { delete consumers[ i ]; }

        int total = 0;
        for( i = 0; i < cons; i++ ) total += sum[ i ];
        cout << "total: " << total << endl;
    }
}
