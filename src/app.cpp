#include "app.h"
#include "Globals.h"

//--------------------------------------------------------------
void app::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);
    
    
    ofxJSInitialize();
    
    Globals::instance()->app                = this;
    Globals::instance()->nsSceneManager     = &sceneManager;
    Globals::instance()->eq                 = &eq;
    Globals::instance()->screenLog          = &screenLog;
    Globals::instance()->animationManager   = &animationManager;
    Globals::instance()->editor             = &editor;
    Globals::instance()->gui                = &guiManager;
    Globals::instance()->dataManager        = &dataManager;
    Globals::instance()->loadingScreen      = &loadingScreen;
    
    ofSoundStreamSetup(0,2,this, 44100, 512, 4);
    eq.setup();
    eq.setRange(16);
        
    sceneManager.setup();
    sceneManager.createScene(ofGetWidth(), ofGetHeight(), &editor);

    sceneManager.getScene(0)->bDrawIds      = true;
    sceneManager.getScene(0)->setCameraMode(1);

    
#ifdef PROD_MODE
    sceneManager.createScene(160, 120, &editorPreview);
    sceneManager.getScene(1)->bEnableDof    = false;
    sceneManager.getScene(1)->bEnableFFSA   = false;
    sceneManager.getScene(1)->bdrawModels   = false;
    sceneManager.getScene(1)->bDrawGrid    = false;
    
    sceneManager.getScene(1)->globalAlpha   = 1.0;
    sceneManager.getScene(1)->setCameraMode(1);
   
#endif
    
    
    guiManager.setup();
    animationManager.setup();
    animationManager.setAnimation(0, 1);
    guiManager.populateAnimations();
    
    
#ifdef PROD_MODE
    animationManager.setAnimation(0, 1);
    animationManager.nextAnimation(1);
#endif
    eq.smooth = .2;
    //eq.setFilterRange(-1);
    
    
    // editor
    
    
    editor.setup(true);
    
    editorPreview.setup(false);
    
    dataManager.setup();
    dataManager.load();
    
   
    
    dmxManager.setup(&sceneManager.getScene(0)->sharpies);
    
    setMode(0);
    
    
    midiManager.setup();
    midiMiniManager.setup();

    
}

//--------------------------------------------------------------
void app::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    editor.update();
    
    editorPreview.update();
    
    midiManager.update();
    midiMiniManager.update();

    
    if(mode == 0 ) {
        
        
        animationManager.update(sceneManager.getNumScenes());
        sceneManager.update();
        dmxManager.update();
        
        
    } else {
        
        
    }
    
    guiManager.update();
   
}

//--------------------------------------------------------------
void app::draw(){
    
    ofDisableSmoothing();
    
    ofBackground(25);
    
    
    if(mode == 0 ) {
        
        sceneManager.draw();
        editorPreview.drawAsPreview(ofGetWidth() * .5, 0, 24.5, true);
        ofSetColor(255);
        screenLog.draw();
        
    } else {
        
        editor.draw();
        
    }
    
    loadingScreen.draw();

}

void app::setMode(int mode) {
    
    this->mode = mode;
    guiManager.setMode(mode);
    
    if(mode == 1 ) {
        
        editor.init();
        editor.stop();
        
        
    } else {
        editor.play();
        editor.exit();
        
    }
    
}


//--------------------------------------------------------------
void app::audioReceived (float * input, int bufferSize, int nChannels){
	
	eq.audioReceived(input, bufferSize);
    
}

//--------------------------------------------------------------
void app::keyPressed(int key){
    
    if(guiManager.isBusy())
        return;
    
    if (key == ' ') {
        animationManager.nextAnimation(0);
    }
      
    if (key == 'f') {
        ofToggleFullscreen();
    }
    
    if (key == OF_KEY_RIGHT && mode == 0) {
        editor.pushAnim(true);
        editorPreview.pushAnim(true);
    }
    
    if(key== OF_KEY_LEFT && mode == 0) {
        editor.popAnim(true);
        editorPreview.popAnim(true);
    }
    
#ifdef PROD_MODE
    if (key == 'p') {
        sceneManager.getScene(1)->bmapAnims     = false;
        animationManager.nextAnimation(1);        
    }
#endif
    
    if (key == 'e') {
        eq.setup();
    }
    
    if (key == 'r' && mode == 0 ) {
        dmxManager.resetAll();
    }

}

//--------------------------------------------------------------
void app::keyReleased(int key){

}

//--------------------------------------------------------------
void app::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void app::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void app::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void app::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void app::windowResized(int w, int h){
    Globals::instance()->nsSceneManager->getScene(0)->onResize(w, h);
}

//--------------------------------------------------------------
void app::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void app::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void app::exit(){
    ofxJSFinalize();
    guiManager.exit();
    dataManager.exit();
}


