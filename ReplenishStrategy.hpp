#pragma once
#include <map>

class InventoryManager; // Forward declaration

class ReplenishStrategy {
public:
    virtual void replenish(InventoryManager* manager, std::map<int, int> itemsToReplenish) = 0;
    virtual ~ReplenishStrategy() {}
};

class ThresholdReplenishStrategy : public ReplenishStrategy {
private:
    int threshold;

public:
    ThresholdReplenishStrategy(int threshold);
    void replenish(InventoryManager* manager, std::map<int, int> itemsToReplenish) override;
};

class WeeklyReplenishStrategy : public ReplenishStrategy {
public:
    WeeklyReplenishStrategy();
    void replenish(InventoryManager* manager, std::map<int, int> itemsToReplenish) override;
};