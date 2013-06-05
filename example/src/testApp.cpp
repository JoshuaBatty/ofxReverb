#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    ofEnableAlphaBlending();
	ofSetupScreen();
	//ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
		
	bufferSize			= 512;
	sampleRate 			= 44100;
	speed				= 0.4f;
	volume				= 0.81f;
	wetness				= 0.5f;
	verbRoomSize		= 0.2f;
	verbDamp			= 0.1f;
	verbWidth			= 0.99f;
	
	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);
	
	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
	
    //Load a sample
	beat.load(ofToDataPath("beat2.wav"));
	beat.getLength();
    
    float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
	
    gui = new ofxUICanvas(0,0,ofGetWidth(),ofGetHeight());
	gui->addWidgetDown(new ofxUILabel("FREE VERB", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_MEDIUM));

    gui->addSpacer(length, 2);
    gui->addSlider("volume", 0.0, 1.0, volume, length, 40);
    gui->addSlider("speed", 0.0, 1.0, speed, length, 40);
    gui->addSpacer(length, 2);
    gui->addSlider("Dry/Wet", 0.0, 1.0, wetness, length, 40);
    gui->addSlider("verbRoomSize", 0.0, 1.0, verbRoomSize, length, 40);
    gui->addSlider("verbDamp", 1.0, 1.0, verbDamp, length, 40);
    gui->addSlider("verbWidth", 0.0, 1.0, verbWidth, length, 40);

    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    
    gui->loadSettings("GUI/guiSettings.xml");

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofNoFill();
	
	// draw the left channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 50, 0);
	
    ofSetColor(225,0,0);
    ofDrawBitmapString("Left Channel", 355, 18);
	
    ofSetLineWidth(1);
    ofRect(350, 0, 750, 200);
	
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (int i = 0; i < lAudio.size(); i++){
        float x =  ofMap(i, 0, lAudio.size(), 350, 1100, true);
        ofVertex(x, 100 -lAudio[i]*180.0f);
    }
    ofEndShape(false);
	
    ofPopMatrix();
	ofPopStyle();
	
	// draw the right channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 250, 0);
	
    ofSetColor(225,0,0);
    ofDrawBitmapString("Right Channel", 355, 18);
	
    ofSetLineWidth(1);
    ofRect(350, 0, 750, 200);
	
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
	
    ofBeginShape();
    for (int i = 0; i < rAudio.size(); i++){
        float x =  ofMap(i, 0, rAudio.size(), 350, 1100, true);
        ofVertex(x, 100 -rAudio[i]*180.0f);
    }
    ofEndShape(false);
	
    ofPopMatrix();
	ofPopStyle();
	

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    gui->toggleVisible();
}

//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
	
	pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;
	
	/* update internal reverb parameter values */
	//	reverb.setmode(GetParameter(KMode));
	reverb.setroomsize(verbRoomSize);
	reverb.setdamp(verbDamp);
	reverb.setwidth(verbWidth);
	reverb.setwet(wetness);
	reverb.setdry(1.0f - wetness);
	
	for (int i = 0; i < bufferSize; i++){
		//SAMPLE
		sample=beat.play(speed, 0, beat.length);
		
		//Assign sample to left and right channels
		float left = sample * volume * leftScale;
		float right = sample * volume * rightScale;
		
		//Pass left and right channels into reverb for processing
		reverb.processreplace(&left, &right, &output[i*nChannels], &output[i*nChannels + 1], 1, 1);

		lAudio[i] = output[i*nChannels    ];
		rAudio[i] = output[i*nChannels + 1];
	}	
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	
    if(name == "volume")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		volume = slider->getScaledValue();
	}
    else if(name == "speed")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		speed = slider->getScaledValue();
	}
    else if(name == "Dry/Wet")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		wetness = slider->getScaledValue();
	}
    else if(name == "verbRoomSize")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		verbRoomSize = slider->getScaledValue();
	}
    else if(name == "verbDamp")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		verbDamp = slider->getScaledValue();
	}
    else if(name == "verbWidth")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		verbWidth = slider->getScaledValue();
	}
    
}



