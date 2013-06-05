#pragma once

#include "ofMain.h"
#include "ofxReverb.h"
#include "ofxUI.h"
#include "ofxMaxim.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
        void keyPressed(int key);

        void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
        ofSoundStream soundStream;

        //GUI
        ofxUICanvas *gui;
        void guiEvent(ofxUIEventArgs &e);
        
        float 	pan;
        int		sampleRate;
        int		bufferSize;
        float 	volume;
        float	speed;
        
        vector <float> lAudio;
        vector <float> rAudio;

        //Samples
        /* stick you maximilian stuff below */
        double sample,outputs[2];
        ofxMaxiMix mymix;
        ofxMaxiSample beat;
        
        //Reverb
        ofxReverb	reverb;
        float		wetness,verbRoomSize,verbDamp,verbWidth;
        float		reverbOUT;
        
};
