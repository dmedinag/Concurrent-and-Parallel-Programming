#include <iostream>
using namespace std;

template<typename T> _Coroutine Binsertsort {
    const T Sentinel;            // value denoting end of set
    T value;                     // communication: value being passed down/up the tree
  public:
    Binsertsort( T Sentinel ) : Sentinel( Sentinel ) {}

    void sort( T value ) {       // value to be sorted
        Binsertsort::value = value;
        resume();
    }

    T retrieve() {               // retrieve sorted value
        resume();
        return value;
    }

  private:
    void main() {
        Binsertsort<int> less = Binsertsort<int>( Sentinel )&, greater = Binsertsort<int>( Sentinel )&;
        for ( ;; ) {
            if ( value < Sentinel ) less.sort( value );
            else greater.sort( value );
            resume();
        }

    }
};

void uMain::main() {
    int unsorted[] = {25, 6, 9, 5, 99, 100, 101, 7};
    int size = 0;
    const int Sentinel = 1;
    Binsertsort<int> root = Binsertsort<int>( Sentinel )&;

    std::cout << "Initial values were: "

    for ( auto value : unsorted ) {
        root.sort( value );
        std::cout << value;
        size++;
    }

    std::cout << std::endl << "And sorted values are: ";

    int sorted[size];
    for ( int i = 0; i < size; i++ ) {
        sorted[ i ] = root.retrieve();
        std::cout << sorted[ i ];
    }

    std::cout << endl;
}
