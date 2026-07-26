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
    tonic actualTonic = tonic::F;
    layout actualLayout = layout::SCALED;
    scaleType actualScaleType = scaleType::MINOR;
    chordShape actualChordShape = chordShape::OFF;
    chordVoicing actualChordVoicing = chordVoicing::FIRST;
};

void pressingChord(musicalType& state, chordShape pressedChord){
    if (pressedChord == state.actualChordShape){
        state.actualChordShape = chordShape::OFF;
    }else {
        state.actualChordShape = pressedChord;
    }
}

int main(){

    musicalType actualState;
    chordShape pressedChord = chordShape::SIXTH;

    pressingChord(actualState, pressedChord);


    return 0;
}
