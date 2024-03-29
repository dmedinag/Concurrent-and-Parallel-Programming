#include <iostream>
using namespace std;

template<typename T> _Coroutine Binsertsort {
    const T Sentinel;            // value denoting end of set
    T value;                     // communication: value being passed down/up the tree
  public:
    Binsertsort( T Sentinel ) : Sentinel( Sentinel ) {
        printf("I new binsertsort was created with Sentinel %d\n", Sentinel);
    }

    void sort( T value ) {       // value to be sorted
        Binsertsort::value = value;
        resume();
    }

    T retrieve() {               // retrieve sorted value
        resume();
        return value;
    }

  private:
    T ownValue;

    void main() {
        Binsertsort<int> less( Sentinel );
        Binsertsort<int> greater( Sentinel );
        ownValue = value;

        // first iteration
        for( ;; ) {
            if( ownValue != Sentinel ) break;
            suspend();
        }
        ownValue = value;
        suspend();

        for ( ;; ) { // sorting
            if ( value == SENTINEL ) break;
            if ( value < ownValue ) { less.sort( value ); }
            else { greater.sort( value ); }
            suspend();
        }

        for ( ;; ) { // retrieving from the left
            resume();
            value = less.retrieve();
            if( value == SENTINEL ) break;
        }

        value = ownValue;

        for ( ;; ) { // retrieving from the right
            resume();
            value = greater.retrieve();
            if( value == SENTINEL ) break;
        }
    }
};

void uMain::main() {
    int unsorted[] = {25, 6, 9, 5, 99, 100, 101, 7};
    int size = 0;
    Binsertsort<int> root( SENTINEL );

    std::cout << "Initial values were: ";

    for ( auto value : unsorted ) {
        root.sort( value );
        std::cout << value << " ";
        size++;
    }

    root.sort( SENTINEL );

    std::cout << std::endl << "And sorted values are: ";

    int sorted[size];
    for ( int i = 0; i < size; i++ ) {
        sorted[ i ] = root.retrieve();
        std::cout << sorted[ i ] << " ";
    }

    std::cout << endl;
}

// void something() {
//     Binsertsort<int> root( Sentinel );
//
//     root.sort(0); // root.value = 0
//     root.sort(27); // root.greater.value = 27
//     root.sort(4); // root.greater.less.value = 4
//     root.sort(-12); // root.less.value = -12
//     root.sort(-3); // root.less.greater.value = -3
//     root.sort(30); // root.greater.greater.value = 30
// }
