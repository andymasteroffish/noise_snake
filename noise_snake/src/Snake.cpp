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
    
    canChangeDir = true;
    
    justDied = false;
    
    trailX.clear();
    trailY.clear();
    for (int i=0; i<5; i++){
        trailX.push_back(curX);
        trailY.push_back(curY);
    }
    
    score = 0;
}

void Snake::update(){
    
    framesToMove = ofMap(score, 0, 70, 13, 4, true);
    
    //tick down the move timer
    moveTimer--;
    
    if (moveTimer <= 0){
        moveTimer = framesToMove;
        canChangeDir = true;
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
    
    //did we touch ourselves
    for (int i=0; i<trailX.size(); i++){
        if (curX == trailX[i] && curY == trailY[i]){
            justDied = true;
        }
    }
    
}

void Snake::eat(){
    trailX.push_back(curX);
    trailY.push_back(curY);
    score++;
}

void Snake::keyPressed(int key){
    if (!canChangeDir){
        return;
    }
    
    if (key == OF_KEY_LEFT || key == 'a' || key == 'A'){
        xDir = -1;
        yDir = 0;
        canChangeDir = false;
    }
    if (key == OF_KEY_RIGHT || key == 'd' || key == 'D'){
        xDir = 1;
        yDir = 0;
        canChangeDir = false;
    }
    if (key == OF_KEY_UP || key == 'w' || key == 'W'){
        xDir = 0;
        yDir = -1;
        canChangeDir = false;
    }
    if (key == OF_KEY_DOWN || key == 's' || key == 'S'){
        xDir = 0;
        yDir = 1;
        canChangeDir = false;
    }
}



