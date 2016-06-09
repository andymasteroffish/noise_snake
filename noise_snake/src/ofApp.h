#pragma once

#include "ofMain.h"
#include "Snake.hpp"
#include "Spark.hpp"

class ofApp : public ofBaseApp{
	public:
		void setup();
    void resetGame();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void audioOut(float * output, int bufferSize, int nChannels);
    
    void setFood();
    void endGame();
    
    int bufferSize;
    
    ofSoundStream soundStream;
    
    vector<float> audioValues;
    
#define GRID_SIZE 16
    bool grid[GRID_SIZE][GRID_SIZE];
    float gridf[GRID_SIZE][GRID_SIZE];
    int cellSize;
    
    int pos;
    
    
    //game stuff
    Snake snake;
    
    int foodX, foodY;
    
    bool gameOver;
    vector<Spark> sparks;
    int resetGameTimer;
    int gameOverTime;
};