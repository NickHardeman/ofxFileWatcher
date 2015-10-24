//
//  ofxFileWatcher.h
//  emptyExample
//
//  Created by Nick Hardeman on 10/23/15.
//

#pragma once
#include "ofMain.h"

class ofxFileWatcher {
public:
    
    enum {
        EVENT_DEL = 0,
        EVENT_MOD
    };
    
    class WatchFile {
    public:
        WatchFile() {
            modTime = 0;
            bDeleted = false;
            tex = NULL;
            img = NULL;
        }
        
        ofImage* img;
        ofTexture* tex;
        
        string filePath;
        string name;
        long modTime;
        bool bDeleted;
    };
    
    class Event {
    public:
        int type;
        string filePath;
        string name;
    };
    
    ofxFileWatcher();
    
    static bool shouldRemoveFile( const shared_ptr<WatchFile>& af );
    
    void addFile( string aPath );
    void addFile( string aPath, string aCuteLilNickName );
    
    void addImage( string aPath, ofImage* aImg );
    void addImage( string aPath, ofImage* aImg, string aCuteLilNickName );
    
    void addTexture( string aPath, ofTexture* aTex );
    void addTexture( string aPath, ofTexture* aTex, string aCuteLilNickName );
    
    void start();
    void stop();
    bool isRunning();
    
    void setInterval( int aNumMillis );
    void removeDeletedFiles();
    // remove deleted files //
    void setGarbageCollection( bool aB );
    
    ofEvent< Event > FileModEvent;
    
protected:
    void add( string aPath, string aCuteLilNickName, ofImage* aImg, ofTexture* aTex );
    void update( ofEventArgs& args );
    
    bool bHasListeners, bAutoRemoveDelFiles;
    int intervalMillis, nextCheckTimeMillis;
    
    vector< shared_ptr<WatchFile> > files;
};
