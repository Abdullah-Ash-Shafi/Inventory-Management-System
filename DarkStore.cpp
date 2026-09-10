#include "DarkStore.hpp"
#include <cmath>
#include <algorithm>

// DarkStore Implementation
DarkStore::DarkStore(std::string n, double x_coord, double y_coord) 
    : replenishStrategy(nullptr) {
    name = n;
    x = x_coord;
    y = y_coord;
    inventoryManager = new InventoryManager(new DbInventoryStore());
}

DarkStore::~DarkStore() {
    delete inventoryManager;
    if (replenishStrategy) delete replenishStrategy;
}

double DarkStore::distanceTo(double ux, double uy) {
    return std::sqrt((x - ux) * (x - ux) + (y - uy) * (y - uy));
}

void DarkStore::runReplenishment(std::map<int, int> itemsToReplenish) {
    if (replenishStrategy) {
        replenishStrategy->replenish(inventoryManager, itemsToReplenish);
    }
}

std::vector<Product*> DarkStore::getAllProducts() {
    return inventoryManager->getAvailableProducts();
}

int DarkStore::checkStock(int sku) {
    return inventoryManager->checkStock(sku);
}

void DarkStore::removeStock(int sku, int qty) {
    inventoryManager->removeStock(sku, qty);
}

void DarkStore::addStock(int sku, int qty) {
    inventoryManager->addStock(sku, qty);
}

void DarkStore::setReplenishStrategy(ReplenishStrategy* strategy) {
    this->replenishStrategy = strategy;
}

std::string DarkStore::getName() const { return name; }
double DarkStore::getXCoordinate() const { return x; }
double DarkStore::getYCoordinate() const { return y; }
InventoryManager* DarkStore::getInventoryManager() { return inventoryManager; }

// DarkStoreManager Singleton
DarkStoreManager* DarkStoreManager::instance = nullptr;

DarkStoreManager::DarkStoreManager() {
    darkStores = new std::vector<DarkStore*>();
}

DarkStoreManager* DarkStoreManager::getInstance() {
    if (instance == nullptr) {
        instance = new DarkStoreManager();
    }
    return instance;
}

void DarkStoreManager::registerDarkStore(DarkStore* ds) {
    darkStores->push_back(ds);
}

std::vector<DarkStore*> DarkStoreManager::getNearbyDarkStores(double ux, double uy, double maxDistance) {
    std::vector<std::pair<double, DarkStore*>> distList;
    for (auto ds : *darkStores) {
        double d = ds->distanceTo(ux, uy);
        if (d <= maxDistance) {
            distList.push_back({d, ds});
        }
    }
    std::sort(distList.begin(), distList.end(),
         [](const auto &a, const auto &b){ return a.first < b.first; });

    std::vector<DarkStore*> result;
    for (auto &p : distList) {
        result.push_back(p.second);
    }
    return result;
}

DarkStoreManager::~DarkStoreManager() {
    for (auto ds : *darkStores) {
        delete ds;
    }
    delete darkStores;
}