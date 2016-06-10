//
//  Spark.hpp
//  emptyExample
//
//  Created by Andrew Wallace on 6/9/16.
//
//

#ifndef Spark_hpp
#define Spark_hpp

#include "ofMain.h"

class Spark{
public:
    
    void setup(int x, int y, int _gridSize);
    void update();
    void move();
    void changeDir();
    
    int gridSize;
    
    int curX, curY;
    int xDir, yDir;
    
    float chanceToChangeDir;
    
    int deathTimer;
    float curVal;
    
    int framesToMove;
    int moveTimer;
    
    
};

#endif /* Spark_hpp */
