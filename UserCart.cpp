#include "UserCart.hpp"
#include <iostream>

// Cart Implementation
void Cart::addItem(int sku, int qty) {
    Product* prod = ProductFactory::createProduct(sku);
    items.push_back({prod, qty});
    std::cout << "[Cart] Added SKU " << sku << " (" << prod->getName() 
              << ") x" << qty << std::endl;
}

double Cart::getTotal() {
    double sum = 0.0;
    for (auto &it : items) {
        sum += (it.first->getPrice() * it.second);
    }
    return sum;
}

std::vector<std::pair<Product*, int>> Cart::getItems() {
    return items;
}

Cart::~Cart() {
    for (auto &it : items) {
        delete it.first;
    }
}

// User Implementation
User::User(std::string n, double x_coord, double y_coord) {
    name = n;
    x = x_coord;
    y = y_coord;
    cart = new Cart();
}

User::~User() {
    delete cart;
}

Cart* User::getCart() {
    return cart;
}

// DeliveryPartner Implementation
DeliveryPartner::DeliveryPartner(std::string n) : name(n) {}