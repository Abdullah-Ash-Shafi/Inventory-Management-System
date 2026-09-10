#include <iostream>
#include "ZeptoHelper.hpp"
#include "UserCart.hpp"
#include "Order.hpp"
#include "DarkStore.hpp"

using namespace std;

int main() {
    // 1) Initialize system data
    ZeptoHelper::initialize();

    // 2) User comes on platform
    User* user = new User("Aditya", 1.0, 1.0);
    cout << "\nUser with name " << user->name << " comes on platform" << endl;

    // 3) Display local products
    ZeptoHelper::showAllItems(user);

    // 4) Add items to cart
    cout << "\nAdding items to cart\n";
    Cart* cart = user->getCart();
    cart->addItem(101, 4);
    cart->addItem(102, 3);
    cart->addItem(103, 2);

    // 5) Place order
    OrderManager::getInstance()->placeOrder(user, user->cart);

    // 6) Clean up allocated dynamic memory
    delete user;
    delete DarkStoreManager::getInstance();

    return 0;
}