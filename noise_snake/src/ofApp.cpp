#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);

    bufferSize = 256;
    soundStream.setup(this, 2, 2, 44100, bufferSize, 4);
    
    audioValues.assign(bufferSize, 0.0);
    
    cellSize = 15;

    gameOverTime = 180;
    showScoreTime = 180;
    
    onColor.set(0,0,0);
    offColor.set(255, 255, 255);
    
    maxVol = 0.4;

    
    sparks.clear();
    
    resetGame();
    
    setTitle();
    onTitle = true;
}

//--------------------------------------------------------------
void ofApp::resetGame(){
    gameOver = false;
    onTitle = false;
    snake.setup(GRID_SIZE);
    resetGameTimer = gameOverTime;
    
    
    int hue = ofRandom(256);
    int hue2 = (hue + 128) % 256;
    
    offColor.setHsb(hue, 50, 250);
    onColor.setHsb(hue2, 250, 100);
    
    ofBackground(offColor);
    
    setFood();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float pulseSpeed = 10;
    
    maxVol = ofMap(snake.score, 0, 40, 0.3, 0.9, true);
    if (onTitle){
        maxVol = 0.1;
    }
    if (gameOver && resetGameTimer < 0){
        maxVol = 0.1;
    }
    
    //turn everythign off
    for (int x=0; x<GRID_SIZE; x++){
        for (int y=0; y<GRID_SIZE; y++){
            grid[x][y] = false;
            gridf[x][y] = 0;
        }
    }
    
    if (!gameOver && !onTitle){
    
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
    
    if (gameOver && !onTitle){
        if (resetGameTimer > 0){
            gridf[snake.curX][snake.curY] = ofGetFrameNum() % 20 > 10;
        }
        else{
            for (int x=0; x<GRID_SIZE; x++){
                for (int y=0; y<GRID_SIZE; y++){
                    if (scoreGrid[x][y]){
                        gridf[x][y] = ofGetFrameNum() % 20 > 10;
                    }
                }
            }

        }
        
        resetGameTimer--;
        if (resetGameTimer <= -showScoreTime){
            onTitle = true;
        }
    }
    
    if (onTitle){
        
        for (int x=0; x<GRID_SIZE; x++){
            for (int y=0; y<GRID_SIZE; y++){
                if (titleGrid[x][y]){
                    //gridf[x][y] = 0.5 + 0.5 * abs(sin(ofGetElapsedTimef() + x*GRID_SIZE+y));
                    gridf[x][y] = ofRandom(0.2,1);// ofRandomuf() < 0.8 ? 1 : 0;
                }
            }
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
                ofColor thisCol = onColor * gridf[x][y] + offColor * (1-gridf[x][y]);
                ofSetColor(thisCol);
            }else{
                ofNoFill();
                ofSetColor(onColor);
            }
            
            ofDrawRectangle(x*cellSize, y*cellSize, cellSize, cellSize);
        }
    }
    
    ofPopMatrix();
    
    //let's draw the nasty sound we're making too
    float graphHeight = 50;
    ofSetColor(onColor);
    for (int i=0; i<bufferSize-1; i++){
        
        float valA = audioValues[i] * graphHeight;
        float valB = audioValues[i+1] * graphHeight;
        
        ofDrawLine(i, ofGetHeight()-valA-5, i+1, ofGetHeight()-valB-5);
            
    }
    
    
    if (onTitle){
        ofSetColor(onColor);
        ofDrawBitmapString("By Andy Wallace 2016", 8, GRID_SIZE*cellSize+12);
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
    
    if (onTitle){
        resetGame();
    }
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
    
    //get the tens one one place of the score
    int firstScoreNum = (int) floor((snake.score/10) * 10) /10;
    int secondScoreNum = snake.score % 10;
    
    //clear the score grid
    for (int x=0; x<GRID_SIZE; x++){
        for (int y=0; y<GRID_SIZE; y++){
            scoreGrid[x][y] = 0;
        }
    }
    
    //add the numbers
    setScoreGrid(firstScoreNum, 3, 4);
    setScoreGrid(secondScoreNum, 9, 4);
    
}


//--------------------------------------------------------------
void ofApp::setTitle(){
    for (int x=0; x<GRID_SIZE; x++){
        for (int y=0; y<GRID_SIZE; y++){
            titleGrid[x][y] = false;
        }
    }
    
    titleGrid[0][2] = true;
    titleGrid[0][3] = true;
    titleGrid[0][4] = true;
    titleGrid[0][5] = true;
    titleGrid[0][6] = true;
    
    titleGrid[1][3] = true;
    titleGrid[1][9] = true;
    titleGrid[1][10] = true;
    titleGrid[1][11] = true;
    titleGrid[1][13] = true;
    
    titleGrid[2][4] = true;
    titleGrid[2][9] = true;
    titleGrid[2][11] = true;
    titleGrid[2][12] = true;
    titleGrid[2][13] = true;
    
    titleGrid[3][2] = true;
    titleGrid[3][3] = true;
    titleGrid[3][4] = true;
    titleGrid[3][5] = true;
    titleGrid[3][6] = true;
    
    titleGrid[4][9] = true;
    titleGrid[4][10] = true;
    titleGrid[4][11] = true;
    titleGrid[4][12] = true;
    titleGrid[4][13] = true;
    
    titleGrid[5][3] = true;
    titleGrid[5][4] = true;
    titleGrid[5][5] = true;
    titleGrid[5][10] = true;
    
    titleGrid[6][2] = true;
    titleGrid[6][6] = true;
    titleGrid[6][11] = true;
    
    titleGrid[7][3] = true;
    titleGrid[7][4] = true;
    titleGrid[7][5] = true;
    titleGrid[7][9] = true;
    titleGrid[7][10] = true;
    titleGrid[7][11] = true;
    titleGrid[7][12] = true;
    titleGrid[7][13] = true;
    
    titleGrid[9][2] = true;
    titleGrid[9][3] = true;
    titleGrid[9][4] = true;
    titleGrid[9][5] = true;
    titleGrid[9][6] = true;
    titleGrid[9][9] = true;
    titleGrid[9][10] = true;
    titleGrid[9][11] = true;
    titleGrid[9][12] = true;
    titleGrid[9][13] = true;
    
    titleGrid[10][10] = true;
    titleGrid[10][11] = true;
    
    titleGrid[11][2] = true;
    titleGrid[11][3] = true;
    titleGrid[11][4] = true;
    titleGrid[11][6] = true;
    titleGrid[11][9] = true;
    titleGrid[11][12] = true;
    titleGrid[11][13] = true;
    
    titleGrid[12][2] = true;
    titleGrid[12][4] = true;
    titleGrid[12][5] = true;
    titleGrid[12][6] = true;
    
    titleGrid[13][9] = true;
    titleGrid[13][10] = true;
    titleGrid[13][11] = true;
    titleGrid[13][12] = true;
    titleGrid[13][13] = true;
    
    titleGrid[14][2] = true;
    titleGrid[14][3] = true;
    titleGrid[14][4] = true;
    titleGrid[14][5] = true;
    titleGrid[14][6] = true;
    titleGrid[14][9] = true;
    titleGrid[14][11] = true;
    titleGrid[14][13] = true;
    
    titleGrid[15][2] = true;
    titleGrid[15][4] = true;
    titleGrid[15][6] = true;
}

//--------------------------------------------------------------
void ofApp::setScoreGrid(int num, int xOffset, int yOffset){
    
    if (num == 0){
        scoreGrid[0+xOffset][1+yOffset] = true;
        scoreGrid[0+xOffset][2+yOffset] = true;
        scoreGrid[0+xOffset][3+yOffset] = true;
        scoreGrid[0+xOffset][4+yOffset] = true;
        scoreGrid[0+xOffset][5+yOffset] = true;
        
        scoreGrid[1+xOffset][0+yOffset] = true;
        scoreGrid[1+xOffset][6+yOffset] = true;
        
        scoreGrid[2+xOffset][0+yOffset] = true;
        scoreGrid[2+xOffset][6+yOffset] = true;
        
        scoreGrid[3+xOffset][1+yOffset] = true;
        scoreGrid[3+xOffset][2+yOffset] = true;
        scoreGrid[3+xOffset][3+yOffset] = true;
        scoreGrid[3+xOffset][4+yOffset] = true;
        scoreGrid[3+xOffset][5+yOffset] = true;
    }
    
    if (num == 1){
        scoreGrid[1+xOffset][1+yOffset] = true;
        scoreGrid[1+xOffset][6+yOffset] = true;
        
        scoreGrid[2+xOffset][0+yOffset] = true;
        scoreGrid[2+xOffset][1+yOffset] = true;
        scoreGrid[2+xOffset][2+yOffset] = true;
        scoreGrid[2+xOffset][3+yOffset] = true;
        scoreGrid[2+xOffset][4+yOffset] = true;
        scoreGrid[2+xOffset][5+yOffset] = true;
        scoreGrid[2+xOffset][6+yOffset] = true;
        
        scoreGrid[3+xOffset][6+yOffset] = true;
    }
    
    if (num == 2){
        scoreGrid[0+xOffset][1+yOffset] = true;
        scoreGrid[0+xOffset][5+yOffset] = true;
        scoreGrid[0+xOffset][6+yOffset] = true;
        
        scoreGrid[1+xOffset][0+yOffset] = true;
        scoreGrid[1+xOffset][4+yOffset] = true;
        scoreGrid[1+xOffset][6+yOffset] = true;
        
        scoreGrid[2+xOffset][0+yOffset] = true;
        scoreGrid[2+xOffset][3+yOffset] = true;
        scoreGrid[2+xOffset][6+yOffset] = true;
        
        scoreGrid[3+xOffset][1+yOffset] = true;
        scoreGrid[3+xOffset][2+yOffset] = true;
        scoreGrid[3+xOffset][6+yOffset] = true;
    }
    
    if (num == 3){
        scoreGrid[0+xOffset][0+yOffset] = true;
        scoreGrid[0+xOffset][6+yOffset] = true;
        
        scoreGrid[1+xOffset][0+yOffset] = true;
        scoreGrid[1+xOffset][3+yOffset] = true;
        scoreGrid[1+xOffset][6+yOffset] = true;
        
        scoreGrid[2+xOffset][0+yOffset] = true;
        scoreGrid[2+xOffset][3+yOffset] = true;
        scoreGrid[2+xOffset][6+yOffset] = true;
        
        scoreGrid[3+xOffset][1+yOffset] = true;
        scoreGrid[3+xOffset][2+yOffset] = true;
        scoreGrid[3+xOffset][4+yOffset] = true;
        scoreGrid[3+xOffset][5+yOffset] = true;
    }
    
    if (num == 4){
        scoreGrid[0+xOffset][0+yOffset] = true;
        scoreGrid[0+xOffset][1+yOffset] = true;
        scoreGrid[0+xOffset][2+yOffset] = true;
        scoreGrid[0+xOffset][3+yOffset] = true;
        
        scoreGrid[1+xOffset][3+yOffset] = true;
        scoreGrid[2+xOffset][3+yOffset] = true;
        
        scoreGrid[3+xOffset][0+yOffset] = true;
        scoreGrid[3+xOffset][1+yOffset] = true;
        scoreGrid[3+xOffset][2+yOffset] = true;
        scoreGrid[3+xOffset][3+yOffset] = true;
        scoreGrid[3+xOffset][4+yOffset] = true;
        scoreGrid[3+xOffset][5+yOffset] = true;
        scoreGrid[3+xOffset][6+yOffset] = true;
    }
    
    if (num == 5){
        scoreGrid[0+xOffset][0+yOffset] = true;
        scoreGrid[0+xOffset][1+yOffset] = true;
        scoreGrid[0+xOffset][2+yOffset] = true;
        scoreGrid[0+xOffset][3+yOffset] = true;
        scoreGrid[0+xOffset][6+yOffset] = true;
        
        scoreGrid[1+xOffset][0+yOffset] = true;
        scoreGrid[1+xOffset][3+yOffset] = true;
        scoreGrid[1+xOffset][6+yOffset] = true;
        
        scoreGrid[2+xOffset][0+yOffset] = true;
        scoreGrid[2+xOffset][3+yOffset] = true;
        scoreGrid[2+xOffset][6+yOffset] = true;
        
        scoreGrid[3+xOffset][0+yOffset] = true;
        scoreGrid[3+xOffset][4+yOffset] = true;
        scoreGrid[3+xOffset][5+yOffset] = true;
    }
    
    if (num == 6){
        scoreGrid[0+xOffset][0+yOffset] = true;
        scoreGrid[0+xOffset][1+yOffset] = true;
        scoreGrid[0+xOffset][2+yOffset] = true;
        scoreGrid[0+xOffset][3+yOffset] = true;
        scoreGrid[0+xOffset][4+yOffset] = true;
        scoreGrid[0+xOffset][5+yOffset] = true;
        scoreGrid[0+xOffset][6+yOffset] = true;
        
        scoreGrid[1+xOffset][0+yOffset] = true;
        scoreGrid[1+xOffset][3+yOffset] = true;
        scoreGrid[1+xOffset][6+yOffset] = true;
        
        scoreGrid[2+xOffset][0+yOffset] = true;
        scoreGrid[2+xOffset][3+yOffset] = true;
        scoreGrid[2+xOffset][6+yOffset] = true;
        
        scoreGrid[3+xOffset][0+yOffset] = true;
        scoreGrid[3+xOffset][3+yOffset] = true;
        scoreGrid[3+xOffset][4+yOffset] = true;
        scoreGrid[3+xOffset][5+yOffset] = true;
        scoreGrid[3+xOffset][6+yOffset] = true;
    }
    
    if (num == 7){
        scoreGrid[0+xOffset][0+yOffset] = true;
        scoreGrid[1+xOffset][0+yOffset] = true;
        scoreGrid[2+xOffset][0+yOffset] = true;
        
        scoreGrid[3+xOffset][0+yOffset] = true;
        scoreGrid[3+xOffset][1+yOffset] = true;
        scoreGrid[3+xOffset][2+yOffset] = true;
        scoreGrid[3+xOffset][3+yOffset] = true;
        scoreGrid[3+xOffset][4+yOffset] = true;
        scoreGrid[3+xOffset][5+yOffset] = true;
        scoreGrid[3+xOffset][6+yOffset] = true;
    }
    
    if (num == 8){
        scoreGrid[0+xOffset][1+yOffset] = true;
        scoreGrid[0+xOffset][2+yOffset] = true;
        scoreGrid[0+xOffset][4+yOffset] = true;
        scoreGrid[0+xOffset][5+yOffset] = true;
        
        scoreGrid[1+xOffset][0+yOffset] = true;
        scoreGrid[1+xOffset][3+yOffset] = true;
        scoreGrid[1+xOffset][6+yOffset] = true;
        
        scoreGrid[2+xOffset][0+yOffset] = true;
        scoreGrid[2+xOffset][3+yOffset] = true;
        scoreGrid[2+xOffset][6+yOffset] = true;
        
        scoreGrid[3+xOffset][1+yOffset] = true;
        scoreGrid[3+xOffset][2+yOffset] = true;
        scoreGrid[3+xOffset][4+yOffset] = true;
        scoreGrid[3+xOffset][5+yOffset] = true;
    }
    
    if (num == 9){
        scoreGrid[0+xOffset][0+yOffset] = true;
        scoreGrid[0+xOffset][1+yOffset] = true;
        scoreGrid[0+xOffset][2+yOffset] = true;
        scoreGrid[0+xOffset][3+yOffset] = true;
        
        scoreGrid[1+xOffset][0+yOffset] = true;
        scoreGrid[1+xOffset][3+yOffset] = true;
        
        scoreGrid[2+xOffset][0+yOffset] = true;
        scoreGrid[2+xOffset][3+yOffset] = true;
        
        scoreGrid[3+xOffset][0+yOffset] = true;
        scoreGrid[3+xOffset][1+yOffset] = true;
        scoreGrid[3+xOffset][2+yOffset] = true;
        scoreGrid[3+xOffset][3+yOffset] = true;
        scoreGrid[3+xOffset][4+yOffset] = true;
        scoreGrid[3+xOffset][5+yOffset] = true;
        scoreGrid[3+xOffset][6+yOffset] = true;
    }
    
}

