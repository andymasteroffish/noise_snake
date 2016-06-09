//
//  Snake.hpp
//  emptyExample
//
//  Created by Andrew Wallace on 6/9/16.
//
//

#ifndef Snake_hpp
#define Snake_hpp

#include "ofMain.h"

class Snake{
public:
    
    void setup(int _gridSize);
    void update();
    
    void move();
    void eat();
    
    void keyPressed(int key);
    
    int curX, curY;
    int xDir, yDir;
    
    int framesToMove;
    int moveTimer;
    
    int gridSize;
    
    vector<int> trailX;
    vector<int> trailY;
    
};

#endif /* Snake_hpp */
