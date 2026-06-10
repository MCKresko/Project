#include "WaveManager.h"

WaveManager::WaveManager()
    : rng(std::random_device{}()),
      spawnDelayRandom(0.60f, 0.95f) {
    wave = 0;
    maxWaves = 20;
    enemiesToSpawn = 0;
    spawnTimer = 0;
    running = false;
}

void WaveManager::startNextWave() {
    if (running || wave >= maxWaves) return;

    wave++;
    enemiesToSpawn = 6 + wave * 2;
    spawnTimer = 0;
    spawnDelay = spawnDelayRandom(rng);
    running = true;
}


void WaveManager::update(float dt, std::vector<Enemy>& enemies, const std::vector<sf::Vector2f>& path) {
    if (!running) return;

    spawnTimer += dt;

    if (spawnTimer >= spawnDelay && enemiesToSpawn > 0) {
        std::string enemyType = chooseEnemyType();

        enemies.push_back(Enemy(enemyType, path));

        enemiesToSpawn--;
        spawnTimer = 0;
        spawnDelay = spawnDelayRandom(rng);
    }

    if (enemiesToSpawn <= 0) {
        bool anyAlive = false;

        for (const Enemy& enemy : enemies) {
            if (enemy.isAlive()) {
                anyAlive = true;
            }
        }

        if (!anyAlive) {
            running = false;
        }
    }
}

std::string WaveManager::chooseEnemyType() {
    int maxType = 0;

    if (wave >= 2) {
        maxType = 1;
    }

    if (wave >= 4) {
        maxType = 2;
    }

    if (wave >= 6) {
        maxType = 3;
    }

    std::uniform_int_distribution<int> enemyTypeRandom(0, maxType);
    int randomType = enemyTypeRandom(rng);

    if (randomType == 0) {
        return "scrap";
    }

    if (randomType == 1) {
        return "spider";
    }

    if (randomType == 2) {
        return "drone";
    }

    return "golem";
}
int WaveManager::getWave() const { return wave; }
int WaveManager::getMaxWaves() const { return maxWaves; }
bool WaveManager::isWaveRunning() const { return running; }
bool WaveManager::allWavesFinished() const { return wave >= maxWaves && !running; }
