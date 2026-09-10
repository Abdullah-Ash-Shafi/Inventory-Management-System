#pragma once
#include <string>
#include <vector>
#include <map>
#include "Inventory.hpp"
#include "ReplenishStrategy.hpp"

class DarkStore {
private:
    std::string name;
    double x, y;
    InventoryManager* inventoryManager;
    ReplenishStrategy* replenishStrategy;

public:
    DarkStore(std::string n, double x_coord, double y_coord);
    ~DarkStore();

    double distanceTo(double ux, double uy);
    void runReplenishment(std::map<int, int> itemsToReplenish);

    std::vector<Product*> getAllProducts();
    int checkStock(int sku);
    void removeStock(int sku, int qty);
    void addStock(int sku, int qty);

    void setReplenishStrategy(ReplenishStrategy* strategy);
    std::string getName() const;
    double getXCoordinate() const;
    double getYCoordinate() const;
    InventoryManager* getInventoryManager();
};

class DarkStoreManager {
private:
    std::vector<DarkStore*>* darkStores;
    static DarkStoreManager* instance;

    DarkStoreManager();

public:
    static DarkStoreManager* getInstance();
    void registerDarkStore(DarkStore* ds);
    std::vector<DarkStore*> getNearbyDarkStores(double ux, double uy, double maxDistance);
    ~DarkStoreManager();
};