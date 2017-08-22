#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxJSON.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345

#define PORTRECEIVE 9000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
        void mouseScrolled(int x, int y, float scrollX, float scrollY);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    vector<ofPolyline> polylines, smoothed, resampled;

    vector<ofVec3f> position;
//    ofVec3f position, dPosition;
    
    
    //    vector<ofxJSONElement> dataFromJson;
    ofxJSONElement dataFromJson;
    int objectNum;
    vector<ofImage> images;
    ofImage jsonImg;
    ofImage outImg;
    
    //distance calc
    int img_w, img_h;
    
    vector <float> posX, posY;
    vector <float> currentPosX, currentPosY;
    vector <float> distance;
    vector <string> pathName;
    vector <string> currentPathNames;
    
    vector<ofImage> outputImages;
    int outputImageNum;
    int maxOutputImageNum;
    ofPolyline poly;
    
    int displayMode;
    
    vector <ofPoint> targetLocation;

    bool drawn = false;
    
    float scrollAmount = 0;
    
    int width, height;
    
    vector<string> imageCaptions;
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
    
    ofTrueTypeFont story;
    
    void sendOscMessage();
    
    int imagesReceived = 0;
        
};
