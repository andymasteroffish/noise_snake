#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);

    bufferSize = 256;
    soundStream.setup(this, 2, 2, 44100, bufferSize, 4);
    
    audioValues.assign(bufferSize, 0.0);
    
    cellSize = 15;

    gameOverTime = 240;
    
    sparks.clear();
    
    resetGame();
}

//--------------------------------------------------------------
void ofApp::resetGame(){
    gameOver = false;
    snake.setup(GRID_SIZE);
    
    setFood();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float pulseSpeed = 10;
    
    //turn everythign off
    for (int x=0; x<GRID_SIZE; x++){
        for (int y=0; y<GRID_SIZE; y++){
            grid[x][y] = false;
            gridf[x][y] = 0;
        }
    }
    
    if (!gameOver){
    
        //did the snake eat the food?
        if (foodX == snake.curX && foodY == snake.curY){
            snake.eat();
            setFood();
        }
        
        //update the snake
        snake.update();
        
        //check if we're dead
        if (snake.justDied){
            snake.justDied = false;
            endGame();
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
        gridf[foodX][foodY] = ofGetFrameNum() % 6 > 2 ? 1 : 0;
        //gridf[foodX][foodY] = 0.5 + 0.5*abs(cos( ofGetElapsedTimef() * pulseSpeed * 3));
    }
    
    if (gameOver){
        gridf[snake.curX][snake.curY] = ofGetFrameNum() % 20 > 10;
        
        resetGameTimer--;
        if (resetGameTimer <= 0){
            resetGame();
        }
    }
    
    //also update and add the sparks if any are present
    for (int i=sparks.size()-1; i>=0; i--){
        sparks[i].update();
        if (sparks[i].deathTimer <= 0){
            sparks.erase( sparks.begin()+i );
        }else{
            gridf[sparks[i].curX][sparks[i].curY] += sparks[i].curVal;
            gridf[sparks[i].curX][sparks[i].curY] = MIN(gridf[sparks[i].curX][sparks[i].curY], 1);
        }
    }
    
    
    
    
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
    
    ofPushMatrix();
    
    ofTranslate(8, 2);
    
    for (int x=0; x<GRID_SIZE; x++){
        for (int y=0; y<GRID_SIZE; y++){
            if (gridf[x][y] > 0){
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
    
    ofPopMatrix();
    
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
    
    if (key == 'e'){
        endGame();
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


//--------------------------------------------------------------
void ofApp::endGame(){
    gameOver = true;
    
    //make some sparks on the snake body
    for (int i=0; i<snake.trailX.size(); i++){
        for (int k=0; k<2; k++){
            Spark thisSpark;
            thisSpark.setup(snake.trailX[i], snake.trailY[i], GRID_SIZE);
            sparks.push_back(thisSpark);
        }
    }
    
    resetGameTimer = gameOverTime;
}




