#pragma once
#include <vector>
#include <map>
#include "Product.hpp"

class InventoryStore {
public:
    virtual ~InventoryStore() {}
    virtual void addProduct(Product* prod, int qty) = 0;
    virtual void removeProduct(int sku, int qty) = 0;
    virtual int checkStock(int sku) = 0;
    virtual std::vector<Product*> listAvailableProducts() = 0;
};

class DbInventoryStore : public InventoryStore {
private:
    std::map<int, int>* stock;             // SKU -> quantity
    std::map<int, Product*>* products;     // SKU -> Product*

public:
    DbInventoryStore();
    ~DbInventoryStore();

    void addProduct(Product* prod, int qty) override;
    void removeProduct(int sku, int qty) override;
    int checkStock(int sku) override;
    std::vector<Product*> listAvailableProducts() override;
};

class InventoryManager {
private:
    InventoryStore* store;

public:
    InventoryManager(InventoryStore* store);
    ~InventoryManager();

    void addStock(int sku, int qty);
    void removeStock(int sku, int qty);
    int checkStock(int sku);
    std::vector<Product*> getAvailableProducts();
};