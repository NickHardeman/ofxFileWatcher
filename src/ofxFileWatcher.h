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
        void set(string aname, string afilepath, bool abAutoLoad) {
            name = aname;
            filePath = afilepath;
            file = ofFilePath::getBaseName(filePath)+"."+ofFilePath::getFileExt(filePath);
            bAutoLoad = abAutoLoad;
        }
        
        virtual bool load() { return false; }
        string file = "";
        
        string filePath = "";
        string name = "";
        bool bAutoLoad=false;
        long modTime = 0;
        bool bDeleted=false;
        
    };
    
    class WatchImage : public WatchFile {
    public:
        
        bool load() {
            if( bAutoLoad && img != NULL ) {
                return ofLoadImage( *img, filePath );
            }
            return false;
        }
        ofImage* img = NULL;
    };
    
    class WatchTexture : public WatchFile {
    public:
        bool load() {
            if( bAutoLoad && tex != NULL ) {
                if( bTex2d ) ofDisableArbTex();
                bool bok = ofLoadImage( *tex, filePath );
                if( bTex2d ) ofEnableArbTex();
                return bok;
            }
            return false;
        }
        ofTexture* tex = NULL;
        bool bTex2d = false;
    };
    
    class WatchShader : public WatchFile {
    public:
        bool load() {
            if( bAutoLoad && shader != NULL ) {
                return shader->load( vertPath, fragPath, geomPath );
            }
            return false;
        }
        
        void setShaderPaths( string aVertPath, string aFragPath, string aGeomPath ) {
            vertPath = aVertPath;
            fragPath = aFragPath;
            geomPath = aGeomPath;
        }
        
        ofShader* shader = NULL;
        
        string vertPath = "";
        string fragPath = "";
        string geomPath = "";
    };
    
    
    class Event {
    public:
        int type;
        string filePath;
        string name;
        string file;
    };
    
    ofxFileWatcher();
    
    static bool shouldRemoveFile( const shared_ptr<WatchFile>& af );
    
    void add( string aPath );
    void add( string aPath, string aCuteLilNickName );
    
    void add( string aPath, ofImage* aImg, bool abAutoLoad=true );
    void add( string aPath, ofImage* aImg, string aCuteLilNickName, bool abAutoLoad=true );
    
    void add( string aPath, ofTexture* aTex, bool abAutoLoad=true, bool abTex2d=false );
    void add( string aPath, ofTexture* aTex, string aCuteLilNickName, bool abAutoLoad=true, bool abTex2d=false );
    
    void add( string aVertPath, string aFragPath, string aGeomPath, ofShader* aShader, bool abAutoLoad=true );
    void add( string aVertPath, string aFragPath, string aGeomPath, ofShader* aShader, string aCuteLilNickName, bool abAutoLoad=true );
    
    void start();
    void stop();
    bool isRunning();
    
    void setInterval( int aNumMillis );
    void removeDeletedFiles();
    // remove deleted files //
    void setGarbageCollection( bool aB );
    
    void reloadAll();
    
    ofEvent< Event > FileModEvent;
    
protected:
//    void add( string aPath, string aCuteLilNickName, ofImage* aImg, ofTexture* aTex, bool abAutoLoad );
    void update( ofEventArgs& args );
    
    bool bHasListeners, bAutoRemoveDelFiles;
    int intervalMillis, nextCheckTimeMillis;
    
    vector< shared_ptr<WatchFile> > files;
};
