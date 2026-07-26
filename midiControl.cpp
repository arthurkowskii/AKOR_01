#include <string>
#include <iostream>

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

std::string chordShapeToString(chordShape chord){
    switch (chord) {
        case chordShape::OFF:
            return "OFF";
        case chordShape::SUS2:
            return "SUS2";
        case chordShape::TRI:
            return "TRI";
        case chordShape::SUS4:
            return "SUS4";
        case chordShape::SIXTH:
            return "SIXTH";
        case chordShape::SEVENTH:
            return "SEVENTH";
        case chordShape::NINTH:
            return "NINTH";
        case chordShape::ELEVENTH:
            return "ELEVENTH";
        case chordShape::THIRTEENTH:
            return "THIRTEENTH";
        default:
            return "UNKNOWN";
    }
}

int main(){

    musicalType actualState;
    chordShape pressedChord = chordShape::SIXTH;

    pressingChord(actualState, pressedChord);
    std::cout << "actual chord shape is : " << chordShapeToString(pressedChord) << "\n";


    return 0;
}
