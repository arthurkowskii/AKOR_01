#include <string>
#include <iostream>

enum class tonic { C, D, E, F, G, A, B };
enum class layout { SCALED, CHROMATIC };
enum class scaleType { MAJOR, MINOR };
enum class chordShape { OFF, SUS2, TRI, SUS4, SIXTH, SEVENTH, NINTH, ELEVENTH, THIRTEENTH };
enum class chordVoicing { FIRST, SECOND, THIRD };
enum class shiftState { ON, OFF };
enum octaveInput { OCTAVE_0 = 1, OCTAVE_1 = 2, OCTAVE_2 = 3, OCTAVE_3 = 4,
    OCTAVE_4 = 5, OCTAVE_5 = 6, OCTAVE_6 = 7, OCTAVE_7 = 8, OCTAVE_8 = 9,
};

struct musicalType{
    tonic actualTonic = tonic::F;
    layout actualLayout = layout::SCALED;
    scaleType actualScaleType = scaleType::MINOR;
    chordShape actualChordShape = chordShape::OFF;
    chordVoicing actualChordVoicing = chordVoicing::FIRST;
    octaveInput actualOctaveInput = octaveInput::OCTAVE_3;
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

std::string octaveInputToString(octaveInput octave){
    switch (octave) {
        case octaveInput::OCTAVE_0:
            return "0";
        case octaveInput::OCTAVE_1:
            return "1";
        case octaveInput::OCTAVE_2:
            return "2";
        case octaveInput::OCTAVE_3:
            return "3";
        case octaveInput::OCTAVE_4:
            return "4";
        case octaveInput::OCTAVE_5:
            return "5";
        case octaveInput::OCTAVE_6:
            return "6";
        case octaveInput::OCTAVE_7:
            return "7";
        case octaveInput::OCTAVE_8:
            return "8";
        default:
            return "UNKNOWN";
    }
}

int main(){

    musicalType actualState;
    chordShape pressedChord = chordShape::SIXTH;

    pressingChord(actualState, pressedChord);
    std::cout << "actual chord shape is : " << chordShapeToString(actualState.actualChordShape) << "\n";
    std::cout << "actual octave input is : " << octaveInputToString(actualState.actualOctaveInput) << "\n";


    return 0;
}
