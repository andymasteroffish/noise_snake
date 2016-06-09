//
//  Spark.cpp
//  emptyExample
//
//  Created by Andrew Wallace on 6/9/16.
//
//

#include "Spark.hpp"


void Spark::setup(int x, int y, int _gridSize){
    gridSize = _gridSize;
    
    curX = x;
    curY = y;
    
    changeDir();
    
    deathTimer = 90;
    
    chanceToChangeDir = 0.2;
    
    framesToMove = 4;
    moveTimer = 1;
    
    
}

void Spark::update(){
    
    moveTimer--;
    if (moveTimer <= 0){
        move();
        moveTimer = framesToMove;
    }
    
    deathTimer--;
    
    curVal = ofRandom(1);
    
    if (ofRandomuf() < chanceToChangeDir){
        changeDir();
    }
    
}

void Spark::move(){
    curX += xDir;
    curY += yDir;
    
    if (curX < 0){
        curX = 0;
        xDir = 1;
    }
    
    if (curX >= gridSize){
        curX = gridSize-1;
        xDir = -1;
    }
    
    if (curY < 0){
        curY = 0;
        yDir = 1;
    }
    
    if (curY >= gridSize){
        curY = gridSize-1;
        yDir = -1;
    }
}

void Spark::changeDir(){
    xDir = ofRandom(-2,2);
    yDir = ofRandom(-2,2);
}