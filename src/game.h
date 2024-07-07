#pragma once 
#include "spaceship.h"
#include <iostream>
#include "obstacle.h"
using namespace std;

class Game{
    public:
        Game();
        ~Game();
        void HandleInput();
        void Update();
        void Draw();
    private:
        Spaceship spaceship;
        vector<Obstacle> CreaeteObstacles();
        void DeleteInactiveLasers();
        vector<Obstacle> obstacles;
};