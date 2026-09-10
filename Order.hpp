#pragma once
#include <vector>
#include <utility>
#include <string>
#include "Product.hpp"
#include "UserCart.hpp"

class Order {
public:
    static int nextId;
    int orderId;
    User* user;
    std::vector<std::pair<Product*, int>> items;
    std::vector<DeliveryPartner*> partners;
    double totalAmount;

    Order(User* u);
};

class OrderManager {
private:
    std::vector<Order*>* orders;
    static OrderManager* instance;

    OrderManager();

public:
    static OrderManager* getInstance();
    void placeOrder(User* user, Cart* cart);
    std::vector<Order*> getAllOrders();
    ~OrderManager();
};