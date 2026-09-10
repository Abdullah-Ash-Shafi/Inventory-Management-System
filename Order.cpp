#include "Order.hpp"
#include "DarkStore.hpp"
#include <iostream>
#include <map>
#include <algorithm>

int Order::nextId = 1;

Order::Order(User* u) {
    orderId = nextId++;
    user = u;
    totalAmount = 0.0;
}

// OrderManager Implementation
OrderManager* OrderManager::instance = nullptr;

OrderManager::OrderManager() {
    orders = new std::vector<Order*>();
}

OrderManager* OrderManager::getInstance() {
    if (instance == nullptr) {
        instance = new OrderManager();
    }
    return instance;
}

void OrderManager::placeOrder(User* user, Cart* cart) {
    std::cout << "\n[OrderManager] Placing Order for: " << user->name << "\n";

    std::vector<std::pair<Product*, int>> requestedItems = cart->getItems();
    double maxDist = 5.0;
    std::vector<DarkStore*> nearbyDarkStores = DarkStoreManager::getInstance()->getNearbyDarkStores(user->x, user->y, maxDist);

    if (nearbyDarkStores.empty()) {
        std::cout << "  No dark stores within 5 KM. Cannot fulfill order.\n";
        return;
    }

    DarkStore* firstStore = nearbyDarkStores.front();
    bool allInFirst = true;

    for (auto& item : requestedItems) {
        int sku = item.first->getSku();
        int qty = item.second;
        if (firstStore->checkStock(sku) < qty) {
            allInFirst = false;
            break;
        }
    }

    Order* order = new Order(user);

    if (allInFirst) {
        std::cout << "  All items at: " << firstStore->getName() << "\n";
        for (auto& item : requestedItems) {
            int sku = item.first->getSku();
            int qty = item.second;
            firstStore->removeStock(sku, qty);
            order->items.push_back({ item.first, qty });
        }
        order->totalAmount = cart->getTotal();
        order->partners.push_back(new DeliveryPartner("Partner1"));
        std::cout << "  Assigned Delivery Partner: Partner1\n";
    } else {
        std::cout << "  Splitting order across stores...\n";
        std::map<int, int> allItems;

        for (auto& item : requestedItems) {
            allItems[item.first->getSku()] = item.second;
        }

        int partnerId = 1;
        for (DarkStore* store : nearbyDarkStores) {
            if (allItems.empty()) break;

            std::cout << "   Checking: " << store->getName() << "\n";
            bool assigned = false;
            std::vector<int> toErase;

            for (auto& [sku, qtyNeeded] : allItems) {
                int availableQty = store->checkStock(sku);
                if (availableQty <= 0) continue;

                int takenQty = std::min(availableQty, qtyNeeded);
                store->removeStock(sku, takenQty);

                std::cout << "     " << store->getName() << " supplies SKU " << sku 
                          << " x" << takenQty << "\n";

                order->items.push_back({ ProductFactory::createProduct(sku), takenQty });

                if (qtyNeeded > takenQty) {
                    allItems[sku] = qtyNeeded - takenQty;
                } else {
                    toErase.push_back(sku);
                }
                assigned = true;
            }

            for (int sku : toErase) allItems.erase(sku);

            if (assigned) {
                std::string pname = "Partner" + std::to_string(partnerId++);
                order->partners.push_back(new DeliveryPartner(pname));
                std::cout << "     Assigned: " << pname << " for " << store->getName() << "\n";
            }
        }

        if (!allItems.empty()) {
            std::cout << "  Could not fulfill:\n";
            for (auto& [sku, qty] : allItems) {
                std::cout << "    SKU " << sku << " x" << qty << "\n";
            }
        }

        double sum = 0;
        for (auto& item : order->items) {
            sum += item.first->getPrice() * item.second;
        }
        order->totalAmount = sum;
    }

    std::cout << "\n[OrderManager] Order #" << order->orderId << " Summary:\n";
    std::cout << "  User: " << user->name << "\n  Items:\n";
    for (auto& item : order->items) {
        std::cout << "    SKU " << item.first->getSku()
                  << " (" << item.first->getName() << ") x" << item.second
                  << " @ ₹" << item.first->getPrice() << "\n";
    }
    std::cout << "  Total: ₹" << order->totalAmount << "\n  Partners:\n";
    for (auto* dp : order->partners) {
        std::cout << "    " << dp->name << "\n";
    }
    std::cout << std::endl;

    orders->push_back(order);

    for (auto* dp : order->partners) delete dp;
    for (auto& item : order->items) delete item.first;
}

std::vector<Order*> OrderManager::getAllOrders() {
    return *orders;
}

OrderManager::~OrderManager() {
    for (auto ord : *orders) {
        delete ord;
    }
    delete orders;
}