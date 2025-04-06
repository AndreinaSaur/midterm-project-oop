#include <iostream>
#include <vector>
#include <limits>
#include <cctype>
#include <string>
using namespace std;

// Validation helpers
bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char ch : name) 
        if (!isalpha(ch) && ch != ' ') return false;
    return true;
}

bool isDigits(const string& str) {
    if (str.empty()) return false;
    for (char ch : str) 
        if (!isdigit(ch)) return false;
    return true;
}

bool isFloat(const string& str) {
    if (str.empty()) return false;
    bool decimalFound = false;
    for (char ch : str) {
        if (!isdigit(ch)) {
            if (ch == '.' && !decimalFound) decimalFound = true;
            else return false;
        }
    }
    return true;
}

// String formatting
string capitalize(string input) {
    if (!input.empty()) {
        input[0] = toupper(input[0]);
        for (size_t i = 1; i < input.size(); ++i)
            input[i] = tolower(input[i]);
    }
    return input;
}

string fixedWidth(const string& str, int width) {
    return (str.length() > width) ? str.substr(0, width-3) + "..." : str + string(width-str.length(), ' ');
}

// Input helpers
void clearBuffer() { cin.ignore(numeric_limits<streamsize>::max(), '\n'); }

int stringToInt(const string& str) {
    int result = 0;
    for (char ch : str) result = result * 10 + (ch - '0');
    return result;
}

float stringToFloat(const string& str) {
    float result = 0.0f;
    size_t decimalPos = str.find('.');
    if (decimalPos == string::npos) {
        for (char ch : str) result = result * 10 + (ch - '0');
    } else {
        for (size_t i = 0; i < decimalPos; ++i) result = result * 10 + (str[i] - '0');
        float fraction = 0.1f;
        for (size_t i = decimalPos+1; i < str.size(); ++i, fraction *= 0.1f)
            result += (str[i] - '0') * fraction;
    }
    return result;
}

string getStringInput(const string& prompt, bool (*validator)(const string&) = nullptr) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) cout << "Error: Input cannot be empty!\n";
        else if (validator && !validator(input)) cout << "Error: Invalid input format!\n";
        else break;
    }
    return input;
}

int getIntInput(const string& prompt, int min = INT_MIN, int max = INT_MAX) {
    int value;
    while (true) {
        string input = getStringInput(prompt, isDigits);
        value = stringToInt(input);
        if (value < min) cout << "Error: Value must be at least " << min << endl;
        else if (value > max) cout << "Error: Value must be at most " << max << endl;
        else break;
    }
    return value;
}

float getFloatInput(const string& prompt, float min = 0.0f) {
    float value;
    while (true) {
        string input = getStringInput(prompt, isFloat);
        value = stringToFloat(input);
        if (value < min) cout << "Error: Value must be at least " << min << endl;
        else break;
    }
    return value;
}

class Product {
    int productID, quantity;
    string name, category;
    float price;
public:
    Product(int id, const string& n, int qty, float p, const string& cat = "General")
        : productID(id), name(capitalize(n)), quantity(qty), price(p), category(capitalize(cat)) {}
    
    // Getters
    int getID() const { return productID; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    float getPrice() const { return price; }
    string getCategory() const { return category; }
    
    // Setters
    void setName(const string& newName) { name = capitalize(newName); }
    void setQuantity(int newQty) { quantity = newQty; }
    void setPrice(float newPrice) { price = newPrice; }
    void setCategory(const string& newCat) { category = capitalize(newCat); }
    
    void display() const {
        cout << "---------------------------------\n"
             << "ID:       " << productID << "\nName:     " << name
             << "\nQuantity: " << quantity << "\nPrice:    " << price
             << "\nCategory: " << category << "\n---------------------------------\n";
    }
    
    void displayRow() const {
        cout << "| " << fixedWidth(to_string(productID), 6) << " | "
             << fixedWidth(name, 20) << " | " << fixedWidth(to_string(quantity), 8) << " | "
             << fixedWidth("₱" + to_string(price), 10) << " | " << fixedWidth(category, 15) << " |\n";
    }
};

class Inventory {
    vector<Product> products;
    int nextID = 1;
public:
    void addProduct(const Product& product) { products.push_back(product); }
    
    Product* findProduct(int id) {
        for (auto& p : products) if (p.getID() == id) return &p;
        return nullptr;
    }
    
    bool removeProduct(int id) {
        for (size_t i = 0; i < products.size(); i++)
            if (products[i].getID() == id) {
                products.erase(products.begin() + i);
                return true;
            }
        return false;
    }
    
    const vector<Product>& getAllProducts() const { return products; }
    
    vector<Product> getLowStockProducts() const {
        vector<Product> lowStock;
        for (const auto& p : products) if (p.getQuantity() <= 5) lowStock.push_back(p);
        return lowStock;
    }
    
    void sortByQuantity(bool ascending = true) {
        for (size_t i = 0; i < products.size(); i++)
            for (size_t j = i+1; j < products.size(); j++)
                if ((ascending && products[i].getQuantity() > products[j].getQuantity()) ||
                   (!ascending && products[i].getQuantity() < products[j].getQuantity()))
                    swap(products[i], products[j]);
    }
    
    void sortByPrice(bool ascending = true) {
        for (size_t i = 0; i < products.size(); i++)
            for (size_t j = i+1; j < products.size(); j++)
                if ((ascending && products[i].getPrice() > products[j].getPrice()) ||
                   (!ascending && products[i].getPrice() < products[j].getPrice()))
                    swap(products[i], products[j]);
    }
    
    int getNextID() { return nextID++; }
    bool isEmpty() const { return products.empty(); }
    
    void displayAll() const {
        if (products.empty()) {
            cout << "No products in inventory.\n";
            return;
        }
        cout << "Inventory List (" << products.size() << " items):\n"
             << "---------------------------------------------------------------------------\n"
             << "| ID     | Name        | Quantity | Price      | Category        |\n"
             << "---------------------------------------------------------------------------\n";
        for (const auto& p : products) p.displayRow();
        cout << "---------------------------------------------------------------------------\n";
    }
    
    void displayLowStock() const {
        auto lowStock = getLowStockProducts();
        if (lowStock.empty()) {
            cout << "No low stock products (No quantity <= 5).\n";
            return;
        }
        cout << "\nLow Stock Products (" << lowStock.size() << " items):\n"
             << "---------------------------------------------------------------------------\n"
             << "| ID     | Name         | Quantity | Price      | Category        |\n"
             << "---------------------------------------------------------------------------\n";
        for (const auto& p : lowStock) p.displayRow();
        cout << "---------------------------------------------------------------------------\n";
    }
};

// Menu functions
void addProductMenu(Inventory& inventory) {
    cout << "\n------------------------------------------\n";
    string name = getStringInput("Enter product name: ", isValidName);
    int quantity = getIntInput("Enter quantity: ", 0);
    float price = getFloatInput("Enter price: ", 0.0f);
    string category = getStringInput("Enter product category: ", isValidName);
    
    inventory.addProduct(Product(
        inventory.getNextID(),
        name,
        quantity,
        price,
        category
    ));
    cout << "\nItem added successfully!\n";
}
void updateProductMenu(Inventory& inventory) {
    if (inventory.isEmpty()) {
        cout << "Inventory is empty. No products to update.\n";
        return;
    }
    cout << "\n------------------------------------------\n";
    int id = getIntInput("Enter product ID to update: ", 1);
    Product* product = inventory.findProduct(id);
    if (!product) {
        cout << "Item not found!\n";
        return;
    }
    
    cout << "\nCurrent product details:\n";
    product->display();
    cout << "\n1. Quantity\n2. Price\n3. Cancel\n";
    int choice = getIntInput("Enter your choice: ", 1, 3);
    
    if (choice == 1) {
        int oldQty = product->getQuantity();
        product->setQuantity(getIntInput("Enter new quantity: ", 0));
        cout << "\nQuantity of Item " << product->getName() << " updated from " 
             << oldQty << " to " << product->getQuantity() << endl;
    } else if (choice == 2) {
        float oldPrice = product->getPrice();
        product->setPrice(getFloatInput("Enter new price: P", 0.0f));
        cout << "\nPrice of Item " << product->getName() << " updated from ₱" 
             << oldPrice << " to ₱" << product->getPrice() << endl;
    }
}

void removeProductMenu(Inventory& inventory) {
    if (inventory.isEmpty()) {
        cout << "\n------------------------------------------\nInventory is empty.\n";
        return;
    }
    cout << "\n------------------------------------------\n";
    int id = getIntInput("Enter product ID to remove: ", 1);
    Product* product = inventory.findProduct(id);
    if (!product) {
        cout << "Item not found!\n";
        return;
    }
    cout << "\nProduct to be removed:\n";
    product->display();
    if (getStringInput("Confirm removal? (y/n): ") == "y") {
        inventory.removeProduct(id);
        cout << "Item removed successfully!\n";
    } else {
        cout << "Removal cancelled.\n";
    }
}

void displayMainMenu() {
    cout << "\n-----<Main Menu>-----\n"
         << "1. Add Item\n2. Update Item\n3. Remove Item\n4. Display All Items\n"
         << "5. Search Item\n6. Sort Items\n7. Display Low Stock Items\n8. Exit\n"
         << "\n---------------------\n";
}

int main() {
    Inventory inventory;
    while (true) {
        displayMainMenu();
        int choice = getIntInput("Enter your choice: ", 1, 8);
        switch (choice) {
            case 1: addProductMenu(inventory); break;
            case 2: updateProductMenu(inventory); break;
            case 3: removeProductMenu(inventory); break;
            case 4: inventory.displayAll(); break;
            case 5: {
                if (inventory.isEmpty()) {
                    cout << "Inventory is empty.\n";
                    break;
                }
                int id = getIntInput("Enter product ID to search: ", 1);
                Product* product = inventory.findProduct(id);
                if (product) {
                    cout << "\nProduct found:\n";
                    product->display();
                } else cout << "Item not found!\n";
                break;
            }
            case 6: {
                if (inventory.isEmpty()) {
                    cout << "\n------------------------------------------\nInventory is empty.\n";
                    break;
                }
                cout << "\n------------------------------------------\n"
                     << "1. Sort by Quantity\n2. Sort by Price\n";
                int sortChoice = getIntInput("Enter your choice: ", 1, 2);
                cout << "\n1. Ascending\n2. Descending\n";
                bool ascending = getIntInput("Enter sort order: ", 1, 2) == 1;
                (sortChoice == 1) ? inventory.sortByQuantity(ascending) : inventory.sortByPrice(ascending);
                cout << "Products sorted (" << (ascending ? "ascending" : "descending") << " order)\n";
                inventory.displayAll();
                break;
            }
            case 7: inventory.displayLowStock(); break;
            case 8: cout << "Exiting program\n"; return 0;
        }
    }
}