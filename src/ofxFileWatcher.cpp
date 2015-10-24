//
//  ofxFileWatcher.cpp
//  emptyExample
//
//  Created by Nick Hardeman on 10/23/15.
//

#include "ofxFileWatcher.h"
#include <sys/stat.h>

//--------------------------------------------------------------
ofxFileWatcher::ofxFileWatcher() {
    bHasListeners = false;
    setGarbageCollection( true );
    setInterval( 1000 );
    start();
}

//--------------------------------------------------------------
bool ofxFileWatcher::shouldRemoveFile( const shared_ptr<WatchFile>& af ) {
    return af->bDeleted;
}

//--------------------------------------------------------------
void ofxFileWatcher::addFile( string aPath ) {
    ofFile tfile( aPath );
    addFile( aPath, tfile.getBaseName() );
}

//--------------------------------------------------------------
void ofxFileWatcher::addFile( string aPath, string aCuteLilNickName ) {
    add( aPath, aCuteLilNickName, NULL, NULL );
}

//--------------------------------------------------------------
void ofxFileWatcher::addImage( string aPath, ofImage* aImg ) {
    ofFile tfile( aPath );
    addImage( aPath, aImg, tfile.getBaseName() );
}

//--------------------------------------------------------------
void ofxFileWatcher::addImage( string aPath, ofImage* aImg, string aCuteLilNickName ) {
    add( aPath, aCuteLilNickName, aImg, NULL );
}

//--------------------------------------------------------------
void ofxFileWatcher::addTexture( string aPath, ofTexture* aTex ) {
    ofFile tfile( aPath );
    addTexture( aPath, aTex, tfile.getBaseName() );
}

//--------------------------------------------------------------
void ofxFileWatcher::addTexture( string aPath, ofTexture* aTex, string aCuteLilNickName ) {
    add( aPath, aCuteLilNickName, NULL, aTex );
}

//--------------------------------------------------------------
void ofxFileWatcher::add( string aPath, string aCuteLilNickName, ofImage* aImg, ofTexture* aTex ) {
    if( !ofFile::doesFileExist( aPath )) {
        ofLogWarning("ofxFileWatcher file does not exist!! - ") << aPath;
        return;
    }
    
    shared_ptr< WatchFile > wf( new WatchFile() );
    wf->filePath    = ofToDataPath(aPath, true);
    wf->name        = aCuteLilNickName;
    wf->img         = aImg;
    wf->tex         = aTex;
    
    if( wf->img != NULL && !wf->img->isAllocated() ) {
        ofLoadImage( *wf->img, wf->filePath );
    }
    if( wf->tex != NULL && !wf->tex->isAllocated() ) {
        ofLoadImage( *wf->tex, wf->filePath );
    }
    
    files.push_back( wf );
}

//--------------------------------------------------------------
void ofxFileWatcher::start() {
    if( !bHasListeners ) {
        ofAddListener( ofEvents().update, this, &ofxFileWatcher::update );
    }
    bHasListeners = true;
}

//--------------------------------------------------------------
void ofxFileWatcher::stop() {
    if( bHasListeners ) {
        ofRemoveListener( ofEvents().update, this, &ofxFileWatcher::update );
    }
    bHasListeners = false;
}

//--------------------------------------------------------------
bool ofxFileWatcher::isRunning() {
    return bHasListeners;
}

//--------------------------------------------------------------
void ofxFileWatcher::setInterval( int aNumMillis ) {
    intervalMillis = aNumMillis;
}

//--------------------------------------------------------------
void ofxFileWatcher::removeDeletedFiles() {
    ofRemove( files, shouldRemoveFile );
}
// remove deleted files //
//--------------------------------------------------------------
void ofxFileWatcher::setGarbageCollection( bool aB ) {
    bAutoRemoveDelFiles = aB;
}

//--------------------------------------------------------------
void ofxFileWatcher::update( ofEventArgs& args ) {
    int emills = ofGetElapsedTimeMillis();
    if( emills >= nextCheckTimeMillis ) {
        // check the files here //
        for( int i = 0; i < files.size(); i++ ) {
            shared_ptr< WatchFile >& wf = files[i];
            if( !ofFile::doesFileExist(wf->filePath) ) {
                if( !wf->bDeleted ) {
                    Event tevent;
                    tevent.filePath = wf->filePath;
                    tevent.name     = wf->name;
                    tevent.type     = EVENT_DEL;
                    ofNotifyEvent( FileModEvent, tevent, this);
                }
                wf->bDeleted = true;
            } else {
                wf->bDeleted = false;
                struct stat attrib;
                stat( wf->filePath.c_str(), &attrib);
                if( wf->modTime <= 0 ) {
                    wf->modTime = attrib.st_mtime;
                }
//                cout << "wf->name: " << wf->name << " time: " << attrib.st_mtime << endl;
                if( wf->modTime != attrib.st_mtime ) {
                    
                    // check to see if we need to load a tex or image //
                    if( wf->img != NULL ) {
                        ofLoadImage( *wf->img, wf->filePath );
                    }
                    
                    if( wf->tex != NULL ) {
                        ofLoadImage( *wf->tex, wf->filePath );
                    }
                    
                    // pass an event here //
                    Event tevent;
                    tevent.filePath = wf->filePath;
                    tevent.name     = wf->name;
                    tevent.type     = EVENT_MOD;
                    ofNotifyEvent( FileModEvent, tevent, this);
                }
            }
        }
        
        if( bAutoRemoveDelFiles ) {
            removeDeletedFiles();
        }
        
        nextCheckTimeMillis = emills + intervalMillis;
    }
}












