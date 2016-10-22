#include <iostream>
#include <fstream>
#include <sstream>
#define DEFAULT_UNIFORM 37
//#define OUTPUT
using namespace std;

int parseArgs( int argc, char *argv[], stringstream *xfile,
                stringstream *yfile, size_t *xr, size_t *xcyr, size_t *yc);
void matrixmultiply( int *Z[], int *X[], unsigned int xr, unsigned int xc, int *Y[], unsigned int yc );
int readFile( stringstream *dest, char *filename );
void fillUniformMatrix( int *dest[], size_t rows, size_t cols, int value );
int fillMatrixFromFile( int *dest[], size_t rows, size_t cols, stringstream *file );
void generateOutput( int *X[], int *Y[], int *Z[], size_t xr, size_t xcyr, size_t yc );

void matrixmultiply( int *Z[], int *X[], unsigned int xr, unsigned int xc, int *Y[], unsigned int yc ) {
    printf("%s\n", "Sí, sí, todo bien");
}


void uMain::main() {

    size_t xr, xcyr, yc;
    stringstream xfile, yfile;

    INIT: {
        // ARGUMENT PARSING

        if( !parseArgs( argc, argv, &xfile, &yfile, &xr, &xcyr, &yc ) ) break INIT;

        // MATRIX INITIALIZATION

            // create the three matrices to be used with the given dimensions
        int *X[ xr ], *Y[ xcyr ], *Z[ xr ];

            // reserve the necessary space for each matrix in the heap
        for( size_t i = 0; i < xr; i++ ) {
            X[ i ] = new int[ xcyr ];
            Z[ i ] = new int[ yc ];
        }
        for( size_t i = 0; i < xcyr; i++ ) { Y[ i ] = new int[ yc ]; }

            // fill the initial matrices with their values
        if( argc == 4 ) {
            // with default values when applicable
            fillUniformMatrix( X, xr, xcyr, DEFAULT_UNIFORM );
            fillUniformMatrix( Y, xcyr, yc, DEFAULT_UNIFORM );
        } else {
            // with values from file when applicable
            fillMatrixFromFile( X, xr, xcyr, &xfile );
            fillMatrixFromFile( Y, xcyr, yc, &yfile );
        }

        // MATRIX MULTIPLICATION

        matrixmultiply( Z, X, xr, xcyr, Y, yc );

        // PRINT OUTPUT

        if( argc == 6 ) { generateOutput( X, Y, Z, xr, xcyr, yc ); }

        // FREE RESOURCES

        for( size_t i = 0; i < xr; i++ ) {
            free( X[ i ] );
            free( Z[ i ] );
        }
        for( size_t i = 0; i < xcyr; i++ ) { free( Y[ i ] ); }
    }
}

// Auxiliary functions

/*
    Safely open a file, and make the stringstream usable with that input.
    Args:
        - dest: where to put the content of the file to be read
        - filename: location of the file to be read
*/
int readFile( stringstream *dest, char *filename ) {
    ifstream file( filename );
    if ( file ) {
        *dest << file.rdbuf();
        file.close();
    } else {
        cerr << "Error! Could not open input file \"" << filename << "\"" << endl;
        return -1;
    }
    return 0;
}

/*
    Fills a matrix with a certain value given its sizes.
    Args:
        - dest: location of the matrix to be filled
        - rows: y dimension of the matrix
        - cols: x dimension of the matrix
        - value: content to be placed in the matrix
*/
void fillUniformMatrix( int *dest[], size_t rows, size_t cols, int value ) {
    for( size_t i = 0; i < rows; i++ ) {
        for( size_t j = 0; j < cols; j++ ) {
            dest[ i ][ j ] = value;
        }
    }
}

/*
    Fills a matrix with the values obtained from an ifstream.
    Args:
        - dest: location of the matrix to be filled
        - rows: y dimension of the matrix
        - cols: x dimension of the matrix
        - file: content to be placed in the matrix
*/
int fillMatrixFromFile( int *dest[], size_t rows, size_t cols, stringstream *file ) {
    for( size_t i = 0; i < rows; i++ ) {
        for( size_t j = 0; j < cols; j++ ) {
            *file >> dest[ i ][ j ];
            if( file->fail() ) {
                cerr << "Error when reading matrix file" << endl;
                return -1;
            }
        }
    }
    return 0;
}

int parseArgs( int argc, char *argv[], stringstream *xfile,
                stringstream *yfile, size_t *xr, size_t *xcyr, size_t *yc) {
    switch ( argc ) {
        case 6: { // when files are provided
            if( readFile( xfile, argv[ 4 ] ) ) return -1;
            if( readFile( yfile, argv[ 5 ] ) ) return -1;
        }
        case 4: { // when number of arguments is correct
            *xr   = atoi( argv[ 1 ] );
            *xcyr = atoi( argv[ 2 ] );
            *yc   = atoi( argv[ 3 ] );
            break;
        }
        default: {
            cerr << "Usage: " << argv[0] << " xrows (> 0)  xycols (> 0)  ";
            cerr << "ycols (> 0)  [ x-matrix-file  y-matrix-file ]" << endl;
            return -1;
        }
    }
    return 0;
}

void generateOutput( int *X[], int *Y[], int *Z[], size_t xr, size_t xcyr, size_t yc ) {

    // print blank space and Y matrix
    for( size_t i = 0; i < xcyr; i++ ) {
        // blank spaces
        for( size_t j = 0; j < xcyr; j++ ) cout << setw(8) << " ";
        // separator
        cout << setw(5) << "|" << " ";
        // row of Y matrix
        for( size_t j = 0; j < yc; j++ ) cout << setw(8) << Y[ i ][ j ];
        cout << endl;
    }
    // print separator
    for( size_t i = 0; i < 8*xcyr+4 ; i++ ) {
        cout << "-";
    }
    cout << "*";
    for( size_t i = 0; i < 8*yc+2 ; i++ ) {
        cout << "-";
    }
    cout << endl;
    // print matrices X and Z
    for( size_t i = 0; i < xr; i++ ) {
        // blank spaces
        for( size_t j = 0; j < xcyr; j++ ) cout << setw(8) << X[ i ][ j ];
        // separator
        cout << setw(5) << "|" << " ";
        // row of Y matrix
        for( size_t j = 0; j < yc; j++ ) cout << setw(8) << Z[ i ][ j ];
        cout << endl;
    }


}

8*xcyr+xcyr+3| # # # #
