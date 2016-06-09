#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);

    bufferSize = 256;
    soundStream.setup(this, 2, 2, 44100, bufferSize, 4);
    
    audioValues.assign(bufferSize, 0.0);
    
    cellSize = 15;
//    for (int x=0; x<GRID_SIZE; x++){
//        for (int y=0; y<GRID_SIZE; y++){
//            grid[x][y] = ofRandomuf() > 0.5;
//        }
//    }
    
    snake.setup(GRID_SIZE);
    
    setFood();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float pulseSpeed = 10;
    
    //did the snake eat the food?
    if (foodX == snake.curX && foodY == snake.curY){
        snake.eat();
        setFood();
    }
    
    //update the snake
    snake.update();
    
    //turn everythign off
    for (int x=0; x<GRID_SIZE; x++){
        for (int y=0; y<GRID_SIZE; y++){
            grid[x][y] = false;
            gridf[x][y] = 0;
        }
    }
    
    //turn on the parts of the snake
    grid[snake.curX][snake.curY] = true;
    gridf[snake.curX][snake.curY] = 1;
    for (int i=0; i<snake.trailX.size(); i++){
        grid[snake.trailX[i]][snake.trailY[i]] = true;
        gridf[snake.trailX[i]][snake.trailY[i]] = 0.5 + 0.5*abs(sin(ofGetElapsedTimef() * pulseSpeed + i *0.2) );
    }
    
    //and the food
    grid[foodX][foodY] = 1;//ofGetFrameNum()%2==0;
    gridf[foodX][foodY] = 0.5 + 0.5*abs(cos( ofGetElapsedTimef() * pulseSpeed * 3));
}



//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    
    float maxVol = 0.45;
    
    for (int i=0; i<bufferSize; i++){
        int relPos = (pos + i)%bufferSize;
        int x = floor(relPos / GRID_SIZE);
        int y = relPos % GRID_SIZE;
        
        output[i*2  ] = gridf[x][y] * maxVol;
        output[i*2+1] = output[i*2];
        
        audioValues[i] = gridf[x][y];
    }
    
    //pos++;
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    for (int x=0; x<GRID_SIZE; x++){
        for (int y=0; y<GRID_SIZE; y++){
            if (grid[x][y]){
                ofFill();
                float thisCol = 0 * gridf[x][y] + 200 * (1-gridf[x][y]);
                ofSetColor(thisCol);
            }else{
                ofNoFill();
                ofSetColor(0);
            }
            
            ofDrawRectangle(x*cellSize, y*cellSize, cellSize, cellSize);
        }
    }
    
    //let's draw the nasty sound we're making too
    float graphHeight = 50;
    ofSetColor(0);
    for (int i=0; i<bufferSize-1; i++){
        
        float valA = audioValues[i] * graphHeight;
        float valB = audioValues[i+1] * graphHeight;
        
        ofDrawLine(i, ofGetHeight()-valA-5, i+1, ofGetHeight()-valB-5);
            
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'c'){
        for (int x=0; x<GRID_SIZE; x++){
            for (int y=0; y<GRID_SIZE; y++){
                grid[x][y] = false;
            }
        }
    }
    
    if (key == 'r'){
        for (int x=0; x<GRID_SIZE; x++){
            for (int y=0; y<GRID_SIZE; y++){
                grid[x][y] = ofRandomuf() > 0.5;
            }
        }
    }
    
    if (key == 'f'){
        setFood();
    }
    
    snake.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    int col = x/cellSize;
    int row = y/cellSize;
    
    if (col < GRID_SIZE && row < GRID_SIZE){
        grid[col][row] = !grid[col][row];
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    int col = x/cellSize;
    int row = y/cellSize;
    
    if (col < GRID_SIZE && row < GRID_SIZE){
        grid[col][row] = !grid[col][row];
    }
    

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

//--------------------------------------------------------------
void ofApp::setFood(){
    bool goodPos = false;
    while(!goodPos){
        goodPos = true;
        
        foodX = ofRandom(GRID_SIZE);
        foodY = ofRandom(GRID_SIZE);
        
        if (foodX == snake.curX && foodY == snake.curY){
            goodPos = false;
        }
        
        for (int i=0; i<snake.trailX.size(); i++){
            if (foodX == snake.trailX[i] && foodY == snake.trailY[i]){
                goodPos = false;
            }
        }
    }
}





