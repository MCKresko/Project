#pragma once
#include <string>
#include <SFML/System.hpp>
#include <vector>
#include <random>
#include "Enemy.h"

class WaveManager {
    public:
    WaveManager();
std::mt19937 rng;
std::uniform_real_distribution<float> spawnDelayRandom;
void update(float dt, std::vector<Enemy>& enemies, const std::vector<sf::Vector2f>& path);
void startNextWave();

int getWave() const;
int getMaxWaves() const;
bool isWaveRunning() const;
bool allWavesFinished() const;

    private:
int wave;
int maxWaves;
int enemiesToSpawn;
float spawnTimer;
bool running;

float spawnDelay;
std::string chooseEnemyType();
};
