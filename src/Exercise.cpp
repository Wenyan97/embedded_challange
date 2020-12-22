#include "Exercise.h"

// constructor
Exercise::Exercise(int angle1, int angle2, int state1, int state2) {
    this->transition_angle1 = angle1;
    this->transition_angle2 = angle2;
    this->state1 = state1;
    this->state2 = state2;
}