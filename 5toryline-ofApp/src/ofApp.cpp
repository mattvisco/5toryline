#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    displayMode = 0;
    img_w = 30;
    img_h = 30;
    maxOutputImageNum = 5;
    outputImageNum = 0;

    //set target position (for displaymode1)
    for(int i=0;i<maxOutputImageNum;i++){
        targetLocation.push_back(ofPoint(50,200*i));
    }
    
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(false);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetFrameRate(60);
    ofBackground(200, 200, 200);
    
    //read json
    string inputFile = "dataset500.json";

    //file open
    //  test one line
    bool parsingSuccessful = dataFromJson.open(inputFile);
    
    //JSON
    for(auto& data : dataFromJson){
        pathName.push_back(data["path"].asString());
        posX.push_back(data["point"][0].asFloat());
        posY.push_back(data["point"][1].asFloat());
        ofImage img;
        img.load(pathName.back());
        images.push_back(img);
    }
    
    sender.setup(HOST, PORT);
    receiver.setup(PORTRECEIVE);
    
    story.load("Georgia.ttf", 14);
    
}

string shrinkString(string str) {
    if (str.size() < 60) {
        return str;
    }
    unsigned int i = 0;
    for(i; i< MIN(70,str.size()); i++) {
        if(str[i] == '.' || str[i] == ',') {
            i++;
            break;
        }
    }
    if (i == 70) {
        for(i; i<str.size(); i++) {
            if(str[i] == ' ') {
                break;
            }
        }
    }
    if(i == str.size()) {
        return str;
    }
    string newStr = str.substr(0, i) + '\n' + shrinkString(str.substr(i+1, str.size()));
    return newStr;
}

string fixString(string str) {
    cout << str << endl;
    vector<string> splitCaptionsOnSentence = ofSplitString(str, ".");
    if (splitCaptionsOnSentence.size() > 1) {
        str = "";
        for(int i = 0; i < MIN(3, splitCaptionsOnSentence.size()-1); i++) {
            str += splitCaptionsOnSentence[i] + '.';
        }
    }
    
    vector<string> splitCaptions = ofSplitString(str, "\n");
    vector<string> newCaptionString;
    for(auto& str : splitCaptions) {
        if (str.size() > 60) {
            string newStr = shrinkString(str);
            newCaptionString.push_back(newStr + '\n');
        } else {
            newCaptionString.push_back(str + '\n');
        }
    }
    string captionStr = "";
    for (auto& str : newCaptionString) {
        captionStr += str;
    }
    return captionStr;
}

//--------------------------------------------------------------
void ofApp::update(){
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        // check for story
        if(m.getAddress() == "/narration"){
//            imageCaptions.clear();
            imagesReceived++;
            displayMode = 3;
            imageCaptions.push_back(fixString(m.getArgAsString(0)));
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(displayMode==0){     // DisplayMode ==0

    ofFill();
    ofSetColor(200, 200, 200);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

        ofSetColor(255,255,255);

    bool doubleImgFlg = false;
        
        int i = 0;
        for(auto& img : images) {
            img.draw(posX[i] * width,posY[i] * height, img_w,img_h);
            i++;
        }
        
    // draw poly line
    if(outputImageNum<maxOutputImageNum){
        poly.clear();

        for(int i=0;i<position.size();i++){
//        cout << position.size() << endl;
            poly.addVertex(position.at(i));
        }
    }
    ofSetLineWidth(2);
    ofColor clr=ofColor::fromHex(0x379392);
    ofSetColor(clr);
//    poly.close(); // to connect first and last point of our shape we need to use ÔcloseÕ function
    poly= poly.getSmoothed(5);
    poly.draw();

    
    // draw text
    if(position.size()>0){
        std::stringstream ss;
        ss << "x = " + ofToString(position.back().x) << std::endl;
        ss << "y = " + ofToString(position.back().y) << std::endl;
//        ofDrawBitmapString(ss.str(),10,20);
        
        ofSetLineWidth(1);
        ofSetColor(100,100,100);
        
        std::stringstream ssdist;

        for(int i=0;i<images.size();i++){
            // calculation distance mousepos to images
            float dd= ofDist(position.back().x,position.back().y, posX.at(i)*width+img_w/2, posY.at(i)*height+img_h/2);
            ssdist << "dist = " + ofToString(dd) << std::endl;
            ofSetColor(255,255,255);
//            ofDrawLine(position.back().x,position.back().y, posX.at(i)+img_w/2, posY.at(i)+img_h/2);
//            ofDrawBitmapString(ofToString(dd),posX.at(i),posY.at(i) );

            // Hit detection
            if(dd<img_w/2){ // if line touch images
                    for(int j=0;j<currentPathNames.size();j++){
                        if(pathName.at(i)==currentPathNames.at(j)){
                            doubleImgFlg = true;        // if hit image exist in output image already
                        }
                    }   //for j

                    if(doubleImgFlg==false && outputImageNum<maxOutputImageNum){
                        ofSetColor(255,0,0);
                        outImg.load(pathName.at(i));
                        outImg.draw(posX.at(i)*ofGetWidth(),posY.at(i)*ofGetHeight(),img_w,img_h);

                        currentPathNames.push_back(pathName.at(i));
                        currentPosX.push_back(posX.at(i)*width);
                        currentPosY.push_back(posY.at(i)*height);
                    outImg.save("./tempImg/out"+ofToString(outputImageNum)+".jpg",OF_IMAGE_QUALITY_BEST);
                        outputImages.push_back(pathName.at(i));
                        outputImageNum++;
                    }
                
                
                doubleImgFlg=false;
                
            } else {
                doubleImgFlg=false;
                ofSetColor(255,255,255);
            }

        }   // for i
        if (outputImageNum >= maxOutputImageNum) {
            displayMode = 1;
            sendOscMessage();
        }
        
//        ofDrawBitmapString(ssdist.str(),10,300);   // show distane for test

    }
    
    }  // displayMode ==0
    else if (displayMode==1){
        
        ofFill();
        ofSetColor(200, 200, 200,40);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        
        vector<ofPoint> points = poly.getVertices();
        
        ofPoint firstPoint = points[0];
        poly.clear();
        
        
        float t = ofGetElapsedTimef();
        
        // create poly points
        for(int i=0;i<currentPathNames.size();i++){
            float xPos = currentPosX.at(i) + 50*sin(3*t + 100*i);
            float yPos = currentPosY.at(i)+ 30*sin(4*t + 150*i);
            poly.addVertex(xPos + img_w/2, yPos + img_h/2);
        }
        
        // draw poly line
        ofSetLineWidth(2);
        ofColor clr=ofColor::fromHex(0x379392);
        ofSetColor(clr);
        poly= poly.getSmoothed(1);
        poly.draw();

        // draw output image
        for(int i=0;i<currentPathNames.size();i++){
            float xPos = currentPosX.at(i) + 50*sin(3*t + 100*i);
            float yPos = currentPosY.at(i)+ 30*sin(4*t + 150*i);
            ofSetColor(255, 255, 255);
            outputImages[i].draw(xPos,yPos,img_w,img_h);
        }
        
    }   //DisplayMode ==1
    else if (displayMode == 3) {
        float imgWidth = 120;
        float imgHeight = 120;
        float spacing = (height - currentPathNames.size() * imgHeight);
        float endsSpacing = spacing / 2;
        
        float xPos = 100 + imgWidth/2;
        float yPos;
        
        ofPolyline connection;
        connection.addVertex(xPos, 0);
        
        for(int i=0;i<currentPathNames.size();i++){
            yPos = (spacing * i) + scrollAmount + imgHeight*i + 100;
            connection.addVertex(xPos, endsSpacing + yPos);
        }
        connection.addVertex(xPos, endsSpacing + yPos + 350);
        

        // fade-in
        ofFill();
        ofSetColor(100, 100, 100,20);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        
        ofSetColor(255);
        connection.draw();
        
        for(int i=0;i<imagesReceived;i++){
            float xPos =   100;
            float yPos = (spacing * i) + scrollAmount + imgHeight*i + 100;
        
            outputImages[i].resize(imgWidth, imgHeight);
            outputImages[i].draw( xPos, endsSpacing + yPos );

            story.drawString(imageCaptions[i], 50 + imgWidth + xPos, endsSpacing + yPos);
        }
    }
    
}

void ofApp::sendOscMessage() {
    string path = "tempImg";
    ofDirectory dir(path);
    ofxOscMessage m;
    m.setAddress("/narrate_story");
    m.addStringArg(dir.getAbsolutePath());
    m.addStringArg(to_string(5));
    sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key==' ' && position.size()>0){
        position.clear();
        currentPathNames.clear();
        outputImageNum=0;
    }
    
    if(key=='s'){
        if(displayMode==0){
        displayMode=1;
            sendOscMessage();
        } else{
        displayMode=0;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    scrollAmount += scrollY * 5;
    scrollAmount = MAX(MIN(0, scrollAmount), -1000);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
 
//    position.set(x,y);
    position.push_back(ofPoint(x, y));
    
    for(int i=0;i<dataFromJson.size();i++){
        distance.push_back(ofDist(x,y, posX.at(i)+img_w/2, posY.at(i)+img_h/2));
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    position.clear();

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
