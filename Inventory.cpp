#include "Inventory.hpp"
#include <iostream>

// DbInventoryStore Implementation
DbInventoryStore::DbInventoryStore() {
    stock = new std::map<int, int>();
    products = new std::map<int, Product*>();
}

DbInventoryStore::~DbInventoryStore() {
    for (auto it : *products) {
        delete it.second;
    }
    delete products;
    delete stock;
}

void DbInventoryStore::addProduct(Product* prod, int qty) {
    int sku = prod->getSku();
    if (products->count(sku) == 0) {
        (*products)[sku] = prod;
    } else {
        delete prod;
    }
    (*stock)[sku] += qty;
}

void DbInventoryStore::removeProduct(int sku, int qty) {
    if (stock->count(sku) == 0) return;

    int currentQuantity = (*stock)[sku];
    int remainingQuantity = currentQuantity - qty;
    if (remainingQuantity > 0) {
        (*stock)[sku] = remainingQuantity;
    } else {
        stock->erase(sku);
    }
}

int DbInventoryStore::checkStock(int sku) {
    if (stock->count(sku) == 0) return 0;
    return (*stock)[sku];
}

std::vector<Product*> DbInventoryStore::listAvailableProducts() {
    std::vector<Product*> available;
    for (auto it : *stock) {
        int sku = it.first;
        int qty = it.second;
        if (qty > 0 && products->count(sku)) {
            available.push_back((*products)[sku]);
        }
    }
    return available;
}

// InventoryManager Implementation
InventoryManager::InventoryManager(InventoryStore* store) {
    this->store = store;
}

InventoryManager::~InventoryManager() {
    delete store;
}

void InventoryManager::addStock(int sku, int qty) {
    Product* prod = ProductFactory::createProduct(sku);
    store->addProduct(prod, qty);
    std::cout << "[InventoryManager] Added SKU " << sku << " Qty " << qty << std::endl;
}

void InventoryManager::removeStock(int sku, int qty) {
    store->removeProduct(sku, qty);
}

int InventoryManager::checkStock(int sku) {
    return store->checkStock(sku);
}

std::vector<Product*> InventoryManager::getAvailableProducts() {
    return store->listAvailableProducts();
}