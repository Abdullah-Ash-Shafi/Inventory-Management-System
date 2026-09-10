#pragma once
#include <string>
#include <vector>
#include <utility>
#include "Product.hpp"

class Cart {
public:
    std::vector<std::pair<Product*, int>> items;

    void addItem(int sku, int qty);
    double getTotal();
    std::vector<std::pair<Product*, int>> getItems();
    ~Cart();
};

class User {
public:
    std::string name;
    double x, y;
    Cart* cart;

    User(std::string n, double x_coord, double y_coord);
    ~User();

    Cart* getCart();
};

class DeliveryPartner {
public:
    std::string name;
    DeliveryPartner(std::string n);
};