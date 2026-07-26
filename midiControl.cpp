#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>

enum class tonic { C, D, E, F, G, A, B };
enum class layout { SCALED, CHROMATIC };
enum class scaleType { MAJOR, MINOR };
enum class chordShape { OFF, SUS2, TRI, SUS4, SIXTH, SEVENTH, NINTH, ELEVENTH, THIRTEENTH };
enum class chordVoicing { FIRST, SECOND, THIRD };
enum class shiftState { ON, OFF };

struct musicalType{
    tonic actualTonic = F;
    layout actualLayout = SCALED;
    scaleType actualScale = MINOR;
    chordShape actualChord = TRI;
    chordVoicing actualVoicing = FIRST;
};

int main(){




    return 0;
}
