#pragma once 
#include "spaceship.h"
#include <iostream>
#include "obstacle.h"
#include "alien.h"
#include "mysteryship.h"
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
        vector<Obstacle> CreateObstacles();
        void DeleteInactiveLasers();
        vector<Obstacle> obstacles;
        vector<Alien> aliens;
        void AlienShootLaser();
        void MoveAliens();
        void MoveDownAliens(int distance);
        vector<Laser> alienLasers;
        vector<Alien> CreateAliens();
        int aliensDirection;
        constexpr static float alienLaserShootInterval=0.35;
        float timeLastAlienFired;
        MysteryShip mysteryship;
};