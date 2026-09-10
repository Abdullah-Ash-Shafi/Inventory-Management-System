#include "ReplenishStrategy.hpp"
#include "Inventory.hpp"
#include <iostream>

ThresholdReplenishStrategy::ThresholdReplenishStrategy(int threshold) {
    this->threshold = threshold;
}

void ThresholdReplenishStrategy::replenish(InventoryManager* manager, std::map<int, int> itemsToReplenish) {
    std::cout << "[ThresholdReplenish] Checking threshold... \n";
    for (auto it : itemsToReplenish) {
        int sku = it.first;
        int qtyToAdd = it.second;
        int current  = manager->checkStock(sku);
        if (current < threshold) {
            manager->addStock(sku, qtyToAdd);
            std::cout << "  -> SKU " << sku << " was " << current 
                      << ", replenished by " << qtyToAdd << "\n";
        }
    }
}

WeeklyReplenishStrategy::WeeklyReplenishStrategy() {}

void WeeklyReplenishStrategy::replenish(InventoryManager* manager, std::map<int, int> itemsToReplenish) {
    std::cout << "[WeeklyReplenish] Weekly replenishment triggered for inventory.\n";
}