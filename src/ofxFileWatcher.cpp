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
    nextCheckTimeMillis = 1000;
    setInterval( 1000 );
}

//--------------------------------------------------------------
bool ofxFileWatcher::shouldRemoveFile( const shared_ptr<WatchFile>& af ) {
    return (!af || af->bDeleted);
}

//--------------------------------------------------------------
void ofxFileWatcher::add( string aPath ) {
    add( aPath, ofFilePath::getBaseName(aPath) );
}

//--------------------------------------------------------------
void ofxFileWatcher::add( string aPath, string aCuteLilNickName ) {
    if( ofFile::doesFileExist(aPath) ) {
        auto wf = make_shared<WatchFile>();
        wf->set(aCuteLilNickName, ofToDataPath( aPath, true), false );
        files.push_back( wf );
    } else {
        ofLogWarning("ofxFileWatch :: add file: file does not exist: ") << aPath;
    }
}

//--------------------------------------------------------------
void ofxFileWatcher::add( string aPath, ofImage* aImg, bool abAutoLoad ) {
    add( aPath, aImg, ofFilePath::getBaseName(aPath), abAutoLoad );
}

//--------------------------------------------------------------
void ofxFileWatcher::add( string aPath, ofImage* aImg, string aCuteLilNickName, bool abAutoLoad ) {
    if(ofFile::doesFileExist(aPath)) {
        auto wf = make_shared<WatchImage>();
        wf->set(aCuteLilNickName, ofToDataPath( aPath, true), abAutoLoad );
        wf->img = aImg;
        if( wf->img != NULL && !wf->img->isAllocated() ) {
            wf->load();
        }
        files.push_back( wf );
    } else {
        ofLogWarning("ofxFileWatch :: add image: file does not exist: ") << aPath;
    }
}

//--------------------------------------------------------------
void ofxFileWatcher::add( string aPath, ofTexture* aTex, bool abAutoLoad, bool abTex2d ) {
    add( aPath, aTex, ofFilePath::getBaseName(aPath), abAutoLoad, abTex2d );
}

//--------------------------------------------------------------
void ofxFileWatcher::add( string aPath, ofTexture* aTex, string aCuteLilNickName, bool abAutoLoad, bool abTex2d ) {
    if(ofFile::doesFileExist(aPath)) {
        auto wf = make_shared<WatchTexture>();
        wf->set(aCuteLilNickName, ofToDataPath( aPath, true), abAutoLoad );
        wf->bTex2d = abTex2d;
        wf->tex = aTex;
        if( wf->tex != NULL && !wf->tex->isAllocated() ) {
            wf->load();
        }
        files.push_back( wf );
    } else {
        ofLogWarning("ofxFileWatch :: add texture: file does not exist: ") << aPath;
    }
}

//--------------------------------------------------------------
void ofxFileWatcher::add( string aVertPath, string aFragPath, string aGeomPath, ofShader* aShader, bool abAutoLoad ) {
    string tnickName = "temp";
    if( aVertPath != "" ) {
        tnickName = ofFilePath::getBaseName(aVertPath);
    } else {
        tnickName = ofFilePath::getBaseName(aFragPath);
    }
    add( aVertPath, aFragPath, aGeomPath, aShader, tnickName, abAutoLoad );
}

//--------------------------------------------------------------
void ofxFileWatcher::add( string aVertPath, string aFragPath, string aGeomPath, ofShader* aShader, string aCuteLilNickName, bool abAutoLoad ) {
    vector< shared_ptr<WatchShader> > newWatchers;
    if( aVertPath != "" ) {
        if( ofFile::doesFileExist(aVertPath) ) {
            auto wf = make_shared<WatchShader>();
            wf->set(aCuteLilNickName, ofToDataPath( aVertPath, true), abAutoLoad);
            newWatchers.push_back( wf );
        }
    }
    if( aFragPath != "" ) {
        if( ofFile::doesFileExist(aFragPath) ) {
            auto wf = make_shared<WatchShader>();
            wf->set(aCuteLilNickName, ofToDataPath( aFragPath, true), abAutoLoad);
            newWatchers.push_back( wf );
        }
    }
    if( aGeomPath != "" ) {
        if( ofFile::doesFileExist(aGeomPath) ) {
            auto wf = make_shared<WatchShader>();
            wf->set(aCuteLilNickName, ofToDataPath( aGeomPath, true), abAutoLoad);
            newWatchers.push_back( wf );
        }
    }
    
    if( newWatchers.size() ) {
        for( int i = 0; i < newWatchers.size(); i++ ) {
            newWatchers[i]->setShaderPaths( aVertPath, aFragPath, aGeomPath );
            newWatchers[i]->shader = aShader;
            if( newWatchers[i]->shader != NULL && !newWatchers[i]->shader->isLoaded() ) {
                newWatchers[i]->load();
            }
        }
        files.insert( files.end(), newWatchers.begin(), newWatchers.end() );
    } else {
        ofLogWarning("ofxFileWatcher :: addShader : no valid shader paths! XXXX ");
    }
}

////--------------------------------------------------------------
//void ofxFileWatcher::add( string aPath, string aCuteLilNickName, ofImage* aImg, ofTexture* aTex, bool abAutoLoad ) {
//    if( !ofFile::doesFileExist( aPath )) {
//        ofLogWarning("ofxFileWatcher file does not exist!! - ") << aPath;
//        return;
//    }
//    
//    auto wf = make_shared<WatchFile>();
//    wf->filePath    = ofToDataPath(aPath, true);
//    wf->name        = aCuteLilNickName;
//    wf->img         = aImg;
//    wf->tex         = aTex;
//    wf->bAutoLoad   = abAutoLoad;
//    
//    if( wf->bAutoLoad && wf->img != NULL && !wf->img->isAllocated() ) {
//        ofLoadImage( *wf->img, wf->filePath );
//    }
//    if( wf->bAutoLoad && wf->tex != NULL && !wf->tex->isAllocated() ) {
//        ofLoadImage( *wf->tex, wf->filePath );
//    }
//    
//    files.push_back( wf );
//}

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
    if( emills >= nextCheckTimeMillis && ofGetFrameNum() > 60 ) {
        // check the files here //
        for( int i = 0; i < files.size(); i++ ) {
            if( i >= (int)files.size()) break;
            shared_ptr< WatchFile > wf = files[i];
            if( !wf ) continue;
            if( wf->filePath == "" || !ofFile::doesFileExist(wf->filePath) ) {
                if( !wf->bDeleted ) {
                    Event tevent;
                    tevent.filePath = wf->filePath;
                    tevent.name     = wf->name;
                    tevent.file     = wf->file;
                    tevent.type     = EVENT_DEL;
                    cout << "WatchDog :: passing a delete file event " << endl;
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
                    
                    if( wf->bAutoLoad ) {
                        bool bOk = wf->load();
//                        ofLogNotice("ofxFileWatcher :: load: " ) << bOk << " file: " << wf->file;
                        cout << ("ofxFileWatcher :: load: " ) << bOk << " file: " << wf->file << " | " << ofGetFrameNum() << endl;
                    }
                    
                    // check to see if we need to load a tex or image //
//                    if( wf->bAutoLoad && wf->img != NULL ) {
//                        ofLoadImage( *wf->img, wf->filePath );
//                    }
//                    
//                    if( wf->bAutoLoad && wf->tex != NULL ) {
//                        ofLoadImage( *wf->tex, wf->filePath );
//                    }
                    
                    wf->modTime = attrib.st_mtime;
                    
                    // pass an event here //
                    Event tevent;
                    tevent.filePath = wf->filePath;
                    tevent.name     = wf->name;
                    tevent.file     = wf->file;
                    tevent.type     = EVENT_MOD;
//                    cout << "WatchDog :: passing a mod file event " << endl;
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












