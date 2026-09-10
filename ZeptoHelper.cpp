#include "ZeptoHelper.hpp"
#include "DarkStore.hpp"
#include "ReplenishStrategy.hpp"
#include <iostream>
#include <map>

void ZeptoHelper::showAllItems(User* user) {
    std::cout << "\n[Zepto] All Available products within 5 KM for " << user->name << ":\n";

    DarkStoreManager* dsManager = DarkStoreManager::getInstance();
    std::vector<DarkStore*> nearbyStores = dsManager->getNearbyDarkStores(user->x, user->y, 5.0);

    std::map<int, double> skuToPrice;
    std::map<int, std::string> skuToName;

    for (DarkStore* darkStore : nearbyStores) {
        std::vector<Product*> products = darkStore->getAllProducts();
        for (Product* product : products) {
            int sku = product->getSku();
            if (skuToPrice.count(sku) == 0) {
                skuToPrice[sku] = product->getPrice();
                skuToName[sku]  = product->getName();
            }
        }
    }

    for (auto& entry : skuToPrice) {
        int sku = entry.first;
        double price = entry.second;
        std::cout << "  SKU " << sku << " - " << skuToName[sku] << " @ ₹" << price << "\n";
    }
}

void ZeptoHelper::initialize() {
    auto dsManager = DarkStoreManager::getInstance();

    DarkStore* darkStoreA = new DarkStore("DarkStoreA", 0.0, 0.0);
    darkStoreA->setReplenishStrategy(new ThresholdReplenishStrategy(3));
    std::cout << "\nAdding stocks in DarkStoreA...." << std::endl;  
    darkStoreA->addStock(101, 5);
    darkStoreA->addStock(102, 2);

    DarkStore* darkStoreB = new DarkStore("DarkStoreB", 4.0, 1.0);
    darkStoreB->setReplenishStrategy(new ThresholdReplenishStrategy(3));
    std::cout << "\nAdding stocks in DarkStoreB...." << std::endl; 
    darkStoreB->addStock(101, 3);
    darkStoreB->addStock(103, 10);

    DarkStore* darkStoreC = new DarkStore("DarkStoreC", 2.0, 3.0);
    darkStoreC->setReplenishStrategy(new ThresholdReplenishStrategy(3));
    std::cout << "\nAdding stocks in DarkStoreC...." << std::endl; 
    darkStoreC->addStock(102, 5);
    darkStoreC->addStock(201, 7);

    dsManager->registerDarkStore(darkStoreA);
    dsManager->registerDarkStore(darkStoreB);
    dsManager->registerDarkStore(darkStoreC);
}