class Exercise {
    private:
        int transition_angle1;//when state1 -> state2, the needed angle
        int transition_angle2;//when state2 -> state1, the needed angle
        int state1;
        int state2;
        
    public:
        Exercise(int transition_angle1, int transition_angle2, int state1, int state2);
        void setTransitionAngle1(int angle) {transition_angle1 = angle;}
        void setTransitionAngle2(int angle) {transition_angle2 = angle;}
        void setState1(int state) {state1 = state;}
        void setState2(int state) {state2 = state;}
        int getTransitionAngle1() {return transition_angle1;}
        int getTransitionAngle2() {return transition_angle2;}
        int getState1() {return state1;}
        int getState2() {return state2;}
};

