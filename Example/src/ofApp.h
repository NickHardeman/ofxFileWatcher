#pragma once

#include "ofMain.h"
#include "ofxFileWatcher.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void onFileModEvent( ofxFileWatcher::Event& aEvent );
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofImage bgImage;
    ofTexture tex;
    ofxFileWatcher watcher;
    
    ofEasyCam cam;
    
    ofLight light;
    ofMesh oMesh, fMesh;
};
