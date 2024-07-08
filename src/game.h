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
        Font font;
        char scoreChar[10];
        void Score();
        int score;
        void GameOver();
        void GameReset();
        bool gameOver;
        Music music;
    private:
        Spaceship spaceship;
        vector<Obstacle> CreateObstacles();
        void DeleteInactiveLasers();
        vector<Obstacle> obstacles;
        vector<Alien> aliens;
        void AlienShootLaser();
        void MoveAliens();
        void CheckForCollisions();
        void MoveDownAliens(int distance);
        vector<Laser> alienLasers;
        vector<Alien> CreateAliens();
        int aliensDirection;
        constexpr static float alienLaserShootInterval=0.35;
        float timeLastAlienFired;
        MysteryShip mysteryship;
        float mysteryShipSpawnInterval;
        float timeLastSpawn;
        Sound explosion;
        Sound laserBeam;
};