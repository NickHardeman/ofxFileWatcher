#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowTitle("openFrameworks Application");
    ofSetFrameRate( 60 );
    watcher.addFile( "bg.jpg", "BG" );
    bgImage.loadImage( "bg.jpg" );
    ofDisableArbTex();
    
    watcher.addTexture( "texture.jpg", &tex );
    
    ofAddListener( watcher.FileModEvent, this, &ofApp::onFileModEvent );
    
    oMesh = ofMesh::sphere( 10, 12, OF_PRIMITIVE_TRIANGLES );
    vector< ofMeshFace > tris = oMesh.getUniqueFaces();
    oMesh.setFromTriangles( tris, true );
    for( int i = 0; i < oMesh.getNumNormals(); i++ ) {
        oMesh.getNormals()[i] *= -1.;
    }
    for( int i = 0; i < oMesh.getNumTexCoords(); i++ ){
        oMesh.getTexCoords()[i].x = ofMap( oMesh.getTexCoords()[i].x, 0, 1, 0, 0.45, true );
        oMesh.getTexCoords()[i].y = ofMap( oMesh.getTexCoords()[i].y, 0, 1, 1, 0., true );
    }
    
    fMesh.load("F.ply");
    fMesh.getTexCoords().clear();
    for( int i = 0; i < fMesh.getNumVertices(); i++ ){
        ofVec2f tcoord;
        tcoord.x = ofMap( fMesh.getVertices()[i].x, -1.6, 2.2, 0.55, 1., true );
        tcoord.y = ofMap( fMesh.getVertices()[i].y, -2., 2., 1., 0., true );
        fMesh.addTexCoord( tcoord );
    }
    
    for( int i = 0; i < fMesh.getVertices().size(); i++ ) {
        fMesh.getVertices()[i] *= 5.f;
    }
    
    cam.setAutoDistance( false );
    cam.setPosition( 0, 10, 40 );
    cam.lookAt( ofVec3f(0,0,0) );
}

//--------------------------------------------------------------
void ofApp::update(){
    light.setPosition( cam.getPosition() );
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor( 255 );
    bgImage.draw(0, 0, ofGetWidth(), ofGetHeight() );
    
    ofEnableDepthTest();
    cam.begin();
    ofEnableLighting();
    light.enable();
    
    tex.bind();
    ofPushMatrix(); {
        ofTranslate( -10, 0 );
        ofRotateY( 180 );
        oMesh.draw();
    } ofPopMatrix();
    
    ofPushMatrix(); {
        ofTranslate( 10, 0 );
        fMesh.draw();
    } ofPopMatrix();
    tex.unbind();
    
    light.disable();
    ofDisableLighting();
    
    cam.end();
    ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::onFileModEvent( ofxFileWatcher::Event& aEvent ) {
    if( aEvent.type == ofxFileWatcher::EVENT_MOD ) {
        if( aEvent.name == "BG" ) {
            bgImage.loadImage( aEvent.filePath );
        } else if( aEvent.name == "texture" ) {
            // this file was added with addTexture, so it will automatically get reloaded //
        }
    } else if( aEvent.type == ofxFileWatcher::EVENT_DEL ) {
        // the file has been deleted //
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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