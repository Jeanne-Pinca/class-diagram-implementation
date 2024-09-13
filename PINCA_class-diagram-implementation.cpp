/*
	code by Pinca, Jeanne Paul Lynne Pinca
	C2A - Object Oriented Programming
	Class Diagram Implementation

*/

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm> 
using namespace std;

// Class for product
class Product {	
private:
	string productName;
	string productIdNumber;
	double productCost;
	
public: 
	// Constructor
	Product(string id, string name, double cost)
		: productIdNumber(id), productName(name), productCost(cost) {}
	
	// Getters
	string getProductId() const { return productIdNumber; }
    string getProductName() const { return productName; }
    double getProductCost() const { return productCost; }

    // Static function to initialize predefined products
    static vector<Product> initializeProducts() {
   		return {
	    	Product("0001", "Sticker", 10),
	    	Product("0002", "Notepad", 30),
	        Product("0003", "Eraser", 50),
	        Product("0004", "Pencil", 25),
	        Product("0005", "Marker", 65)
	    };
	}

    // Display the product catalog
	static void displayCatalog(const vector<Product>& products) {
		cout << "\n> Here are the available items." << endl;
		
		cout << string(40, '-') << endl;
        cout << setw(10) << left << "Product ID"
             << setw(20) << left << "Name"
             << setw(10) << right << "Price" << endl;
        cout << string(40, '-') << endl;
        
        for (const auto& product : products) {
            cout << setw(10) << left << product.getProductId()
                 << setw(20) << left << product.getProductName()
                 << setw(10) << right << product.getProductCost() << endl;    
		}
	}
	
	// Search for a product by ID or name
	static const Product* productSearch(const vector<Product>& products, const string& query) {
		string lowerQuery = query;
		transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

        for (const auto& product : products) {
            string id = product.getProductId();
            string name = product.getProductName();
            transform(id.begin(), id.end(), id.begin(), ::tolower);
            transform(name.begin(), name.end(), name.begin(), ::tolower);

            if (id == lowerQuery || name == lowerQuery) {
            	return &product;
            }
        }
        return nullptr;
    }
};

// Class for order
class Order {	
private:
	string orderId;
	vector<pair<Product, int>> orderedItems; // Pair of product and quantity
	double orderTotal;
	static int nextOrderId;  // Static member to track the next order ID

public: 
	// Constructor
	Order(const vector<pair<Product, int>>& items) : orderId(to_string(nextOrderId++)), orderedItems(items), orderTotal(0) {
        for (const auto& item : orderedItems) {
            orderTotal += item.first.getProductCost() * item.second;
        }
    }
    
    // Getter for orderId
	string getOrderId() const { return orderId; }	    

	// Display the order
    void displayOrder() const {
        cout << "\nOrder ID: " << orderId << endl;
        cout << "Total Amount: " << orderTotal  << " PHP" << endl;
        cout << setw(10) << left << "Product ID"
             << setw(20) << left << "Name"
             << setw(10) << right << "Price"
             << setw(10) << right << "Quantity" << endl;
        cout << string(50, '-') << endl;

        for (const auto& item : orderedItems) {
            const Product& product = item.first;
            int quantity = item.second;
            cout << setw(10) << left << product.getProductId()
                 << setw(20) << left << product.getProductName()
                 << setw(10) << right << product.getProductCost()
                 << setw(10) << right << quantity << endl;
        }
    }
    
    static void viewOrders(const vector<Order>& orders) {
        if (orders.empty()) {
            cout << "=========================================================\n";
            cout << "                ORDERS LIST\n";
            cout << "=========================================================\n";
            cout << "> No orders have been made yet! Please checkout purchases from the shopping cart.\n";
            system("pause");
            system("CLS");
        } else {
            cout << "=========================================================\n";
            cout << "                ORDERS LIST\n";
            cout << "=========================================================\n";
            for (const auto& order : orders) {
                order.displayOrder();
                cout << endl;
            }
            
            cout << "> Go back to menu . . . " << endl;
            system("pause");
            system("CLS");
    	}
	}
};

int Order::nextOrderId = 1;

// Class for shopping cart
class ShoppingCart {
private:
	vector<pair<Product, int>> cartItems;

public: 
	void addProduct(const Product& product) {
        for (auto& item : cartItems) {
            if (item.first.getProductId() == product.getProductId()) {
                item.second++;
                return;
            }
        }
        cartItems.push_back(make_pair(product, 1));
    }

	// Display cart
	void displayCart() const {
        if (cartItems.empty()) {
            cout << "=========================================================\n";
            cout << "	     	      SHOPPING CART\n";
            cout << "=========================================================\n";
            cout << "> Your shopping cart is empty!" << endl;
            system("pause");
            system("CLS");
            return;
        }

        cout << "=========================================================\n";
        cout << "	     	      SHOPPING CART\n";
        cout << "=========================================================\n";
        cout << setw(10) << left << "Product ID"
             << setw(20) << left << "Name"
             << setw(10) << right << "Price"
             << setw(10) << right << "Quantity\n";
        cout << "---------------------------------------------------------\n";

        for (const auto& item : cartItems) {
            cout << setw(10) << left << item.first.getProductId()
                 << setw(20) << left << item.first.getProductName()
                 << setw(10) << right << item.first.getProductCost()
                 << setw(10) << right << item.second << endl;
        }

        cout << "\nTotal Price: " << calculateTotal() << " PHP" << endl;
    }

	double calculateTotal() const {
        double total = 0;
        for (const auto& item : cartItems) {
            total += item.first.getProductCost() * item.second;
        }
        return total;
    }

    // Checkout items in the cart and create an order
	void checkout(vector<Order>& orders) {
		if (cartItems.empty()) {
        return;  // No checkout if cart is empty, return to main menu
    	}
		
        char checkoutChoice;
        cout << "\n> Do you want to check out all of the products?" << endl;
		cout << "\n(Y/N): ", cin >> checkoutChoice;

        if (checkoutChoice == 'Y' || checkoutChoice == 'y') {
            if (!cartItems.empty()) {
                Order newOrder(cartItems);
                orders.push_back(newOrder);  // Add the new order to the orders list
                cout << "\n> Order made successfully with Order ID: " << newOrder.getOrderId() << "!\n";
                cartItems.clear();  // Clear the cart after checkout
                system("pause");
                system("CLS");
            } else {
                cout << "\n> Your cart is empty. Cannot proceed with checkout.\n";
            }
   		} else {
            cout << "\n> Checkout canceled. Returning to the menu ...\n";
            system("pause");
            system("CLS");
    	}
	}
};

// helper function to add product to cart
void viewProducts(vector<Product>& products, ShoppingCart& cart) {
	int choice;
    cout << "========================================\n";
    cout << "	    PRODUCT CATALOGUE\n";
    cout << "========================================\n";
    cout << "1 - View All Products\n";
    cout << "2 - Search Product\n";
    cout << "\nEnter your choice: ";

        // Input validation for menu choice
        while (!(cin >> choice) || choice < 1 || choice > 2) {
            cin.clear();  // Clear the error flag on cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
            cout << "> Invalid input! Please enter a number between 1 and 2" << endl;
            cout <<  "\nEnter your choice: ";
        }
        
    	// After valid input, clear the newline character in the buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1) {
    	system("CLS");
        Product::displayCatalog(products);

        string productId;
        while (true) {
            cout << "\n> Enter the product ID of the item you wish to add in your shopping cart"  << endl;;
            cout << "\nID: ", cin >> productId;

            const Product* product = Product::productSearch(products, productId);
            if (product != nullptr) {
                cart.addProduct(*product);
                cout << "\n> Product added successfully! Do you want to add another product?" << endl;
				cout << "\n(Y/N): ";
                char addMore;
                cin >> addMore;
                if (addMore == 'N' || addMore == 'n') break;

				
            } else {
                cout << "> The ID does not exist, try again.\n";
            }
        }
           system("CLS");
           
    } else if (choice == 2) {
    	system("CLS");
        string searchId;
        cout << "> Please enter the ID or Name of the product\n" << endl;
        cout << "Name / ID: ", cin >> searchId;
        const Product* product = Product::productSearch(products, searchId);
        if (product != nullptr) {
            cout << "Product ID\tName\t\tPrice\n";
            cout << product->getProductId() << "\t\t"
                 << product->getProductName() << "\t\t"
                 << product->getProductCost() << endl;
        } else {
            cout << "> Product not found.\n";
        }
        
        system("pause");
        system("CLS");
    }
}

int main() {
	vector<Product> products = Product::initializeProducts();
	ShoppingCart cart;
	vector<Order> orders;
	int choice;

	do {
        cout << "\n=========================================\n";
        cout << "                 E-COMMERCE\n";
        cout << "=========================================\n";
        cout << "1 - View Products\n";
        cout << "2 - View Shopping Cart\n";
        cout << "3 - View Orders\n";
        cout << "4 - Exit\n";
        cout << "\nEnter your choice: ";
        
        // Input validation for menu choice
        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cin.clear();  // Clear the error flag on cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
            cout << "> Invalid input! Please enter a number between 1 and 4" << endl;
            cout <<  "\nEnter your choice: ";
        }
        
    	// After valid input, clear the newline character in the buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
            	system("CLS");
                viewProducts(products, cart);
                break;

            case 2:
            	system("CLS");
                cart.displayCart();
                cart.checkout(orders);
                break;

            case 3:
            	system("CLS");
                Order::viewOrders(orders);  
                break;

            case 4:
                cout << "> Exiting program...\n";
                break;

        }
    } while (choice != 4);

	return 0;
}
