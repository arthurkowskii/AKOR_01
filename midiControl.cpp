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
    tonic tonic = tonic::F;
    layout layout = layout::SCALED;
    scaleType scaleType = scaleType::MINOR;
    chordShape chordShape = chordShape::OFF;
    chordVoicing chordVoicing = chordVoicing::FIRST;
};

int main(){

    musicalType actualState;
    actualState.chordShape = chordShape::TRI;


    return 0;
}
