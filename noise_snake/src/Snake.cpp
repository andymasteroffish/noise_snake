//
//  Snake.cpp
//  emptyExample
//
//  Created by Andrew Wallace on 6/9/16.
//
//

#include "Snake.hpp"


void Snake::setup(int _gridSize){
    gridSize = _gridSize;
    
    curX = _gridSize/2;
    curY = _gridSize/2;
    
    xDir = 1;
    yDir = 0;
    
    framesToMove = 10;
    moveTimer = framesToMove;
    
    trailX.clear();
    trailY.clear();
    for (int i=0; i<5; i++){
        trailX.push_back(curX);
        trailY.push_back(curY);
    }
}

void Snake::update(){
    
    //tick down the move timer
    moveTimer--;
    
    if (moveTimer <= 0){
        moveTimer = framesToMove;
        move();
    }
    
    //keep it in bounds
    if (curX < 0)          curX = gridSize-1;
    if (curX >= gridSize)  curX = 0;
    if (curY < 0)          curY = gridSize-1;
    if (curY >= gridSize)  curY = 0;
    
    
    
}

void Snake::move(){
    
    //bump the tail down
    for (int i=1; i<trailX.size(); i++){
        trailX[i-1] = trailX[i];
        trailY[i-1] = trailY[i];
    }
    trailX[ trailX.size()-1 ] = curX;
    trailY[ trailY.size()-1 ] = curY;
    
    //move
    curX += xDir;
    curY += yDir;
    
    
}

void Snake::eat(){
    trailX.push_back(curX);
    trailY.push_back(curY);
}

void Snake::keyPressed(int key){
    if (key == OF_KEY_LEFT){
        xDir = -1;
        yDir = 0;
    }
    if (key == OF_KEY_RIGHT){
        xDir = 1;
        yDir = 0;
        
    }
    if (key == OF_KEY_UP){
        xDir = 0;
        yDir = -1;
        
    }
    if (key == OF_KEY_DOWN){
        xDir = 0;
        yDir = 1;
    }
}



