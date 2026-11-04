#include "TeaHouse.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

namespace {
    string readLine(const string& prompt) {
        cout << prompt;
        string value;
        getline(cin, value);
        return value;
    }

    string cleanField(const string& value) {
        string result = value;
        replace(result.begin(), result.end(), '|', '/');
        return result;
    }

    string cleanItemName(const string& value) {
        string result = cleanField(value);
        for (char& ch : result) {
            if (ch == ',' || ch == ':') ch = '/';
        }
        return result;
    }

    int readInt(const string& prompt, int minValue = 0, int maxValue = 1000000000) {
        while (true) {
            string line = readLine(prompt);
            stringstream ss(line);
            int value;
            char extra;
            if (ss >> value && !(ss >> extra) && value >= minValue && value <= maxValue)
                return value;
            cout << "Invalid input. Please enter an integer from "
                 << minValue << " to " << maxValue << ".\n";
        }
    }

    double readDouble(const string& prompt, double minValue = 0) {
        while (true) {
            string line = readLine(prompt);
            stringstream ss(line);
            double value;
            char extra;
            if (ss >> value && !(ss >> extra) && value >= minValue)
                return value;
            cout << "Invalid number. Please try again.\n";
        }
    }

    bool validTime(const string& value) {
        if (value.size() != 5 || value[2] != ':') return false;
        if (!isdigit(static_cast<unsigned char>(value[0])) ||
            !isdigit(static_cast<unsigned char>(value[1])) ||
            !isdigit(static_cast<unsigned char>(value[3])) ||
            !isdigit(static_cast<unsigned char>(value[4]))) return false;
        int h = stoi(value.substr(0, 2));
        int m = stoi(value.substr(3, 2));
        return h >= 0 && h <= 23 && m >= 0 && m <= 59;
    }

    bool validDate(const string& value) {
        if (value.size() != 10 || value[4] != '-' || value[7] != '-') return false;
        for (int i = 0; i < 10; ++i) {
            if (i == 4 || i == 7) continue;
            if (!isdigit(static_cast<unsigned char>(value[i]))) return false;
        }
        int y = stoi(value.substr(0, 4));
        int m = stoi(value.substr(5, 2));
        int d = stoi(value.substr(8, 2));
        if (y < 2000 || m < 1 || m > 12 || d < 1) return false;
        const int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        int maxDay = days[m - 1];
        bool leap = (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
        if (m == 2 && leap) maxDay = 29;
        return d <= maxDay;
    }

    int toMinutes(const string& time) {
        return stoi(time.substr(0, 2)) * 60 + stoi(time.substr(3, 2));
    }

    vector<string> split(const string& line, char delimiter = '|') {
        vector<string> fields;
        string item;
        stringstream ss(line);
        while (getline(ss, item, delimiter)) fields.push_back(item);
        if (!line.empty() && line.back() == delimiter) fields.push_back("");
        return fields;
    }

    string money(double value) {
        stringstream ss;
        ss << fixed << setprecision(0) << value << " VND";
        return ss.str();
    }

    void printLine() {
        cout << string(90, '-') << "\n";
    }
}

TeaHouseApp::TeaHouseApp() {
    loadAll();
}

void TeaHouseApp::loadAll() {
    ifstream userFile("users.txt");
    if (!userFile.good()) {
        ofstream out("users.txt");
        out << "U001|owner|123456|Tea House Owner|Owner\n";
        out << "U002|staff01|123456|Tea House Staff|Staff\n";
        out << "U003|artist01|123456|Tea Artist|Artist\n";
        out << "U004|customer01|123456|Sample Customer|Customer\n";
        out.close();
    }
    userFile.close();

    ifstream roomFile("rooms.txt");
    if (!roomFile.good()) {
        ofstream out("rooms.txt");
        out << "R001|Phong Sen|4|500000|Available|Traditional private tea room\n";
        out << "R002|Phong Truc|6|700000|Available|Quiet room with bamboo decor\n";
        out << "R003|Phong Moc|10|1000000|Available|Large group tea room\n";
        out.close();
    }
    roomFile.close();

    ifstream productFile("products.txt");
    if (!productFile.good()) {
        ofstream out("products.txt");
        out << "P001|Oolong Tea - Pot (8-10g)|Tea|Pot|150000|30\n";
        out << "P002|Thiet Quan Am - Pot (8-10g)|Tea|Pot|220000|25\n";
        out << "P003|Aged White Tea with Agarwood - Pot (8-10g)|Tea|Pot|350000|15\n";
        out << "P004|Oriental Beauty - Pot (8-10g)|Tea|Pot|300000|15\n";
        out << "P005|Oolong Tea - 50g|Tea|50g|250000|20\n";
        out << "P006|Oolong Tea - 100g|Tea|100g|450000|15\n";
        out << "P007|Oolong Tea - 200g|Tea|200g|850000|10\n";
        out << "P008|Oolong Tea - Tea Cake 357g|Tea|357g|1400000|8\n";
        out << "P009|Oolong Tea - 1kg|Tea|1kg|3500000|5\n";
        out << "P010|Thiet Quan Am - 50g|Tea|50g|350000|20\n";
        out << "P011|Thiet Quan Am - 100g|Tea|100g|650000|15\n";
        out << "P012|Thiet Quan Am - 200g|Tea|200g|1200000|10\n";
        out << "P013|Thiet Quan Am - Tea Cake 357g|Tea|357g|2000000|8\n";
        out << "P014|Thiet Quan Am - 1kg|Tea|1kg|4800000|5\n";
        out << "P015|Aged White Tea with Agarwood - 50g|Tea|50g|600000|12\n";
        out << "P016|Aged White Tea with Agarwood - 100g|Tea|100g|1100000|10\n";
        out << "P017|Aged White Tea with Agarwood - 200g|Tea|200g|2100000|8\n";
        out << "P018|Aged White Tea with Agarwood - Tea Cake 357g|Tea|357g|3500000|5\n";
        out << "P019|Aged White Tea with Agarwood - 1kg|Tea|1kg|8500000|3\n";
        out << "P020|Oriental Beauty - 50g|Tea|50g|500000|12\n";
        out << "P021|Oriental Beauty - 100g|Tea|100g|950000|10\n";
        out << "P022|Oriental Beauty - 200g|Tea|200g|1800000|8\n";
        out << "P023|Oriental Beauty - Tea Cake 357g|Tea|357g|3000000|5\n";
        out << "P024|Oriental Beauty - 1kg|Tea|1kg|7200000|3\n";
        out << "T001|Tu Sa Teapot|Utensil|Piece|1800000|5\n";
        out << "T002|Porcelain Teapot|Utensil|Piece|650000|8\n";
        out << "T003|Gaiwan Set|Utensil|Set|850000|8\n";
        out << "T004|Fairness Pitcher (Tong)|Utensil|Piece|250000|15\n";
        out << "T005|Tea Cup Set|Utensil|Set|450000|12\n";
        out.close();
    }
    productFile.close();

    auto loadUsers = [&]() {
        ifstream f("users.txt"); string line;
        while (getline(f, line)) {
            auto v = split(line);
            if (v.size() >= 5) users.push_back({v[0],v[1],v[2],v[3],v[4]});
        }
    };
    auto loadRooms = [&]() {
        ifstream f("rooms.txt"); string line;
        while (getline(f, line)) {
            auto v = split(line);
            if (v.size() >= 6) {
                try { rooms.push_back({v[0],v[1],v[5],v[4],stoi(v[2]),stod(v[3])}); }
                catch (...) {}
            }
        }
    };
    auto loadProducts = [&]() {
        ifstream f("products.txt"); string line;
        while (getline(f, line)) {
            auto v = split(line);
            if (v.size() >= 6) {
                try { products.push_back({v[0],v[1],v[2],v[3],stod(v[4]),stoi(v[5])}); }
                catch (...) {}
            }
        }
    };
    auto loadBookings = [&]() {
        ifstream f("bookings.txt"); string line;
        while (getline(f, line)) {
            auto v = split(line);
            if (v.size() >= 8) {
                try { bookings.push_back({v[0],v[1],v[2],v[3],v[4],v[5],v[7],stod(v[6])}); }
                catch (...) {}
            }
        }
    };
    auto loadOrders = [&]() {
        ifstream f("orders.txt"); string line;
        while (getline(f, line)) {
            auto v = split(line);
            if (v.size() >= 5) {
                try { orders.push_back({v[0],v[1],v[2],v[4],stod(v[3])}); }
                catch (...) {}
            }
        }
    };
    auto loadSessions = [&]() {
        ifstream f("sessions.txt"); string line;
        while (getline(f, line)) {
            auto v = split(line);
            if (v.size() >= 6) sessions.push_back({v[0],v[1],v[2],v[3],v[4],v[5]});
        }
    };

    loadUsers();
    loadRooms();
    loadProducts();
    loadBookings();
    loadOrders();
    loadSessions();
}

void TeaHouseApp::saveUsers() {
    ofstream f("users.txt");
    for (const auto& x : users)
        f << x.id << "|" << x.username << "|" << x.password << "|"
          << x.fullName << "|" << x.role << "\n";
}
void TeaHouseApp::saveRooms() {
    ofstream f("rooms.txt");
    for (const auto& x : rooms)
        f << x.id << "|" << x.name << "|" << x.capacity << "|"
          << x.hourlyPrice << "|" << x.status << "|" << x.description << "\n";
}
void TeaHouseApp::saveProducts() {
    ofstream f("products.txt");
    for (const auto& x : products)
        f << x.id << "|" << x.name << "|" << x.category << "|"
          << x.unit << "|" << x.price << "|" << x.stock << "\n";
}
void TeaHouseApp::saveBookings() {
    ofstream f("bookings.txt");
    for (const auto& x : bookings)
        f << x.id << "|" << x.customer << "|" << x.roomId << "|"
          << x.date << "|" << x.startTime << "|" << x.endTime << "|"
          << x.hourlyPrice << "|" << x.status << "\n";
}
void TeaHouseApp::saveOrders() {
    ofstream f("orders.txt");
    for (const auto& x : orders)
        f << x.id << "|" << x.customer << "|" << x.items << "|"
          << x.total << "|" << x.status << "\n";
}
void TeaHouseApp::saveSessions() {
    ofstream f("sessions.txt");
    for (const auto& x : sessions)
        f << x.id << "|" << x.artist << "|" << x.title << "|"
          << x.date << "|" << x.startTime << "|" << x.description << "\n";
}

string TeaHouseApp::nextId(char prefix, const vector<string>& ids) const {
    int maxNumber = 0;
    for (const auto& id : ids) {
        if (id.size() > 1 && id[0] == prefix) {
            try { maxNumber = max(maxNumber, stoi(id.substr(1))); }
            catch (...) {}
        }
    }
    stringstream ss;
    ss << prefix << setw(3) << setfill('0') << maxNumber + 1;
    return ss.str();
}

bool TeaHouseApp::roomExists(const string& id) const {
    for (const auto& x : rooms) if (x.id == id) return true;
    return false;
}
bool TeaHouseApp::productExists(const string& id) const {
    for (const auto& x : products) if (x.id == id) return true;
    return false;
}

bool TeaHouseApp::bookingTimeAvailable(const string& roomId, const string& date,
                                       const string& start, const string& end) const {
    int newStart = toMinutes(start), newEnd = toMinutes(end);
    for (const auto& b : bookings) {
        if (b.roomId == roomId && b.date == date &&
            b.status != "Cancelled" && b.status != "Completed") {
            int oldStart = toMinutes(b.startTime), oldEnd = toMinutes(b.endTime);
            if (newStart < oldEnd && oldStart < newEnd) return false;
        }
    }
    return true;
}

void TeaHouseApp::run() {
    mainMenu();
}

User* TeaHouseApp::authenticate() {
    string username = readLine("Username: ");
    string password = readLine("Password: ");
    for (auto& user : users) {
        if (user.username == username && user.password == password)
            return &user;
    }
    return nullptr;
}

void TeaHouseApp::mainMenu() {
    while (true) {
        cout << "\n============================================\n";
        cout << "          TEA HOUSE MANAGEMENT SYSTEM\n";
        cout << "============================================\n";
        cout << "1. Login\n2. Continue as Guest\n0. Exit\n";
        int choice = readInt("Choose: ", 0, 2);
        if (choice == 0) {
            cout << "Thank you for using Tea House Management System.\n";
            return;
        }
        if (choice == 2) {
            guestMenu();
            continue;
        }
        User* user = authenticate();
        if (!user) {
            cout << "Login failed. Invalid username or password.\n";
            continue;
        }
        cout << "\nWelcome, " << user->fullName << " (" << user->role << ").\n";
        if (user->role == "Owner") ownerMenu(*user);
        else if (user->role == "Staff") staffMenu(*user);
        else if (user->role == "Artist") artistMenu(*user);
        else if (user->role == "Customer") customerMenu(*user);
        else cout << "Unknown account role.\n";
    }
}

void TeaHouseApp::ownerMenu(const User& user) {
    while (true) {
        cout << "\n========== OWNER MENU ==========\n"
             << "1. Manage users\n2. Manage rooms\n3. Manage bookings\n"
             << "4. Manage tea products\n5. Manage tea utensils\n"
             << "6. Manage orders and invoices\n7. Manage tea sessions\n"
             << "8. Revenue report\n0. Logout\n";
        int c = readInt("Choose: ", 0, 8);
        switch (c) {
            case 1: manageUsers(); break;
            case 2: manageRooms(); break;
            case 3: manageBookings(user); break;
            case 4: manageProducts("Tea"); break;
            case 5: manageProducts("Utensil"); break;
            case 6: manageOrders(user); viewInvoices(user); break;
            case 7: manageSessions(user); break;
            case 8: revenueReport(); break;
            case 0: return;
        }
    }
}

void TeaHouseApp::staffMenu(const User& user) {
    while (true) {
        cout << "\n========== STAFF MENU ==========\n"
             << "1. View rooms\n2. Manage bookings\n3. Create/manage orders\n"
             << "4. View invoices\n5. View tea sessions\n0. Logout\n";
        int c = readInt("Choose: ", 0, 5);
        switch (c) {
            case 1: listRooms(); break;
            case 2: manageBookings(user); break;
            case 3: manageOrders(user); break;
            case 4: viewInvoices(user); break;
            case 5: listSessions(); break;
            case 0: return;
        }
    }
}

void TeaHouseApp::artistMenu(const User& user) {
    while (true) {
        cout << "\n========== ARTIST MENU ==========\n"
             << "1. View my sessions\n2. Add a session\n3. Delete my session\n0. Logout\n";
        int c = readInt("Choose: ", 0, 3);
        switch (c) {
            case 1: listSessions(user.username); break;
            case 2: addSession(user); break;
            case 3: deleteSession(user); break;
            case 0: return;
        }
    }
}

void TeaHouseApp::customerMenu(const User& user) {
    while (true) {
        cout << "\n========== CUSTOMER MENU ==========\n"
             << "1. View rooms\n2. Book a room\n3. View my bookings\n"
             << "4. Cancel my booking\n5. View tea products\n6. Buy products\n"
             << "7. View my orders/invoices\n8. View tea sessions\n0. Logout\n";
        int c = readInt("Choose: ", 0, 8);
        switch (c) {
            case 1: listRooms(); break;
            case 2: createBooking(user.username); break;
            case 3: listBookings(user.username); break;
            case 4: cancelBooking(user.username); break;
            case 5: listProducts("Tea"); break;
            case 6: createOrder(user.username); break;
            case 7: listOrders(user.username); viewInvoices(user); break;
            case 8: listSessions(); break;
            case 0: return;
        }
    }
}

void TeaHouseApp::guestMenu() {
    while (true) {
        cout << "\n========== GUEST MENU ==========\n"
             << "1. View rooms\n2. View tea products\n3. View tea utensils\n"
             << "4. View tea sessions\n0. Back\n";
        int c = readInt("Choose: ", 0, 4);
        switch (c) {
            case 1: listRooms(); break;
            case 2: listProducts("Tea"); break;
            case 3: listProducts("Utensil"); break;
            case 4: listSessions(); break;
            case 0: return;
        }
    }
}

void TeaHouseApp::manageUsers() {
    while (true) {
        cout << "\n========== USER MANAGEMENT ==========\n"
             << "1. List users\n2. Add user\n0. Back\n";
        int c = readInt("Choose: ", 0, 2);
        if (c == 0) return;
        if (c == 1) {
            printLine();
            for (const auto& u : users)
                cout << u.id << " | " << u.username << " | "
                     << u.fullName << " | " << u.role << "\n";
            printLine();
        } else {
            string username = cleanField(readLine("Username: "));
            bool exists = false;
            for (const auto& u : users) if (u.username == username) exists = true;
            if (username.empty() || exists) {
                cout << "Username is empty or already exists.\n";
                continue;
            }
            string password = cleanField(readLine("Password: "));
            string name = cleanField(readLine("Full name: "));
            cout << "1. Staff  2. Artist  3. Customer\n";
            int roleChoice = readInt("Role: ", 1, 3);
            string role = roleChoice == 1 ? "Staff" : roleChoice == 2 ? "Artist" : "Customer";
            vector<string> ids;
            for (const auto& u : users) ids.push_back(u.id);
            users.push_back({nextId('U', ids), username, password, name, role});
            saveUsers();
            cout << "User added successfully.\n";
        }
    }
}

void TeaHouseApp::manageRooms() {
    while (true) {
        cout << "\n========== ROOM MANAGEMENT ==========\n"
             << "1. Display rooms\n2. Add room\n3. Edit room\n4. Delete room\n0. Back\n";
        int c = readInt("Choose: ", 0, 4);
        if (c == 0) return;
        if (c == 1) listRooms();
        if (c == 2) addRoom();
        if (c == 3) editRoom();
        if (c == 4) deleteRoom();
    }
}

void TeaHouseApp::listRooms() const {
    cout << "\nROOM LIST\n";
    printLine();
    cout << left << setw(10) << "ID" << setw(20) << "Name"
         << setw(12) << "Capacity" << setw(18) << "Price/hour"
         << setw(16) << "Status" << "Description\n";
    printLine();
    if (rooms.empty()) cout << "No rooms found.\n";
    for (const auto& r : rooms)
        cout << left << setw(10) << r.id << setw(20) << r.name
             << setw(12) << r.capacity << setw(18) << fixed << setprecision(0)
             << r.hourlyPrice << setw(16) << r.status << r.description << "\n";
    printLine();
}

void TeaHouseApp::addRoom() {
    vector<string> ids; for (const auto& r : rooms) ids.push_back(r.id);
    string id = nextId('R', ids);
    string name = cleanField(readLine("Room name: "));
    int capacity = readInt("Capacity: ", 1, 500);
    double price = readDouble("Hourly price (VND): ", 0);
    string desc = cleanField(readLine("Description: "));
    rooms.push_back({id,name,desc,"Available",capacity,price});
    saveRooms();
    cout << "Room added. ID: " << id << "\n";
}

void TeaHouseApp::editRoom() {
    string id = readLine("Room ID: ");
    for (auto& r : rooms) if (r.id == id) {
        r.name = cleanField(readLine("New name: "));
        r.capacity = readInt("New capacity: ", 1, 500);
        r.hourlyPrice = readDouble("New hourly price: ", 0);
        r.description = cleanField(readLine("New description: "));
        cout << "1. Available  2. Maintenance\n";
        r.status = readInt("Status: ", 1, 2) == 1 ? "Available" : "Maintenance";
        saveRooms();
        cout << "Room updated.\n";
        return;
    }
    cout << "Room not found.\n";
}

void TeaHouseApp::deleteRoom() {
    string id = readLine("Room ID to delete: ");
    for (const auto& b : bookings) {
        if (b.roomId == id && b.status == "Confirmed") {
            cout << "Cannot delete a room with a confirmed booking.\n";
            return;
        }
    }
    auto it = remove_if(rooms.begin(), rooms.end(), [&](const Room& r){ return r.id == id; });
    if (it == rooms.end()) { cout << "Room not found.\n"; return; }
    rooms.erase(it, rooms.end());
    saveRooms();
    cout << "Room deleted.\n";
}

void TeaHouseApp::manageProducts(const string& category) {
    while (true) {
        cout << "\n========== " << (category == "Tea" ? "TEA PRODUCT" : "TEA UTENSIL")
             << " MANAGEMENT ==========\n"
             << "1. Display\n2. Add\n3. Edit\n4. Delete\n0. Back\n";
        int c = readInt("Choose: ", 0, 4);
        if (c == 0) return;
        if (c == 1) listProducts(category);
        if (c == 2) addProduct(category);
        if (c == 3) editProduct(category);
        if (c == 4) deleteProduct(category);
    }
}

void TeaHouseApp::listProducts(const string& category) const {
    cout << "\n" << (category == "Tea" ? "TEA PRODUCTS" : "TEA UTENSILS") << "\n";
    printLine();
    cout << left << setw(10) << "ID" << setw(43) << "Name" << setw(10)
         << "Unit" << setw(18) << "Price" << "Stock\n";
    printLine();
    bool found = false;
    for (const auto& p : products) if (p.category == category) {
        found = true;
        cout << left << setw(10) << p.id << setw(43) << p.name
             << setw(10) << p.unit << setw(18) << fixed << setprecision(0)
             << p.price << p.stock << "\n";
    }
    if (!found) cout << "No products found.\n";
    printLine();
}

void TeaHouseApp::addProduct(const string& category) {
    vector<string> ids;
    for (const auto& p : products) if (p.category == category) ids.push_back(p.id);
    char prefix = category == "Tea" ? 'P' : 'T';
    string id = nextId(prefix, ids);
    string name = cleanItemName(readLine("Product name: "));
    string unit = cleanField(readLine("Unit (Pot 8-10g / 50g / 100g / 200g / 357g / 1kg / Set / Piece): "));
    double price = readDouble("Price: ", 0);
    int stock = readInt("Initial stock: ", 0, 1000000);
    products.push_back({id,name,category,unit,price,stock});
    saveProducts();
    cout << "Product added. ID: " << id << "\n";
}

void TeaHouseApp::editProduct(const string& category) {
    string id = readLine("Product ID: ");
    for (auto& p : products) if (p.id == id && p.category == category) {
        p.name = cleanItemName(readLine("New name: "));
        p.unit = cleanField(readLine("New unit: "));
        p.price = readDouble("New price: ", 0);
        p.stock = readInt("New stock: ", 0, 1000000);
        saveProducts();
        cout << "Product updated.\n";
        return;
    }
    cout << "Product not found in this category.\n";
}

void TeaHouseApp::deleteProduct(const string& category) {
    string id = readLine("Product ID: ");
    auto it = remove_if(products.begin(), products.end(),
        [&](const Product& p){ return p.id == id && p.category == category; });
    if (it == products.end()) { cout << "Product not found.\n"; return; }
    products.erase(it, products.end());
    saveProducts();
    cout << "Product deleted.\n";
}

void TeaHouseApp::manageBookings(const User& user) {
    while (true) {
        cout << "\n========== BOOKING MANAGEMENT ==========\n"
             << "1. Display bookings\n2. Create booking\n3. Cancel booking\n"
             << "4. Complete booking\n0. Back\n";
        int c = readInt("Choose: ", 0, 4);
        if (c == 0) return;
        if (c == 1) listBookings(user.role == "Customer" ? user.username : "");
        if (c == 2) {
            string customer = user.role == "Customer" ? user.username : cleanField(readLine("Customer username/name: "));
            createBooking(customer);
        }
        if (c == 3) cancelBooking(user.role == "Customer" ? user.username : "");
        if (c == 4 && user.role != "Customer") completeBooking();
        if (c == 4 && user.role == "Customer") cout << "Customers cannot complete bookings.\n";
    }
}

void TeaHouseApp::createBooking(const string& customer) {
    if (rooms.empty()) { cout << "No rooms are available in the system.\n"; return; }
    listRooms();
    string roomId = readLine("Room ID: ");
    auto roomIt = find_if(rooms.begin(), rooms.end(), [&](const Room& r){ return r.id == roomId; });
    if (roomIt == rooms.end() || roomIt->status != "Available") {
        cout << "Room does not exist or is under maintenance.\n";
        return;
    }
    string date = readLine("Date (YYYY-MM-DD): ");
    string start = readLine("Start time (HH:MM): ");
    string end = readLine("End time (HH:MM): ");
    if (!validDate(date) || !validTime(start) || !validTime(end) ||
        toMinutes(end) <= toMinutes(start)) {
        cout << "Invalid date/time. End time must be after start time.\n";
        return;
    }
    if (!bookingTimeAvailable(roomId, date, start, end)) {
        cout << "This room already has a booking during that time.\n";
        return;
    }
    vector<string> ids; for (const auto& b : bookings) ids.push_back(b.id);
    string id = nextId('B', ids);
    bookings.push_back({id,cleanField(customer),roomId,date,start,end,"Confirmed",roomIt->hourlyPrice});
    saveBookings();
    int duration = toMinutes(end) - toMinutes(start);
    int billableHours = (duration + 59) / 60;
    cout << "Booking created. Booking ID: " << id << "\n";
    cout << "Estimated charge: " << money(billableHours * roomIt->hourlyPrice)
         << " (" << billableHours << " billable hour(s)).\n";
}

void TeaHouseApp::listBookings(const string& customerFilter) const {
    cout << "\nBOOKING LIST\n";
    printLine();
    cout << left << setw(9) << "ID" << setw(20) << "Customer" << setw(9)
         << "Room" << setw(13) << "Date" << setw(9) << "Start"
         << setw(9) << "End" << setw(16) << "Estimate" << "Status\n";
    printLine();
    bool found = false;
    for (const auto& b : bookings) {
        if (!customerFilter.empty() && b.customer != customerFilter) continue;
        found = true;
        int duration = toMinutes(b.endTime) - toMinutes(b.startTime);
        int hours = (duration + 59) / 60;
        cout << left << setw(9) << b.id << setw(20) << b.customer
             << setw(9) << b.roomId << setw(13) << b.date
             << setw(9) << b.startTime << setw(9) << b.endTime
             << setw(16) << fixed << setprecision(0) << hours * b.hourlyPrice
             << b.status << "\n";
    }
    if (!found) cout << "No bookings found.\n";
    printLine();
}

void TeaHouseApp::cancelBooking(const string& customerFilter) {
    string id = readLine("Booking ID to cancel: ");
    for (auto& b : bookings) {
        if (b.id == id && (customerFilter.empty() || b.customer == customerFilter)) {
            if (b.status != "Confirmed") { cout << "Only confirmed bookings can be cancelled.\n"; return; }
            b.status = "Cancelled";
            saveBookings();
            cout << "Booking cancelled.\n";
            return;
        }
    }
    cout << "Booking not found or you do not have permission.\n";
}

void TeaHouseApp::completeBooking() {
    string id = readLine("Booking ID to complete: ");
    for (auto& b : bookings) {
        if (b.id == id) {
            if (b.status != "Confirmed") { cout << "Only confirmed bookings can be completed.\n"; return; }
            b.status = "Completed";
            saveBookings();
            int duration = toMinutes(b.endTime) - toMinutes(b.startTime);
            int hours = (duration + 59) / 60;
            cout << "Booking completed. Amount due: " << money(hours * b.hourlyPrice) << "\n";
            return;
        }
    }
    cout << "Booking not found.\n";
}

void TeaHouseApp::manageOrders(const User& user) {
    while (true) {
        cout << "\n========== ORDER MANAGEMENT ==========\n"
             << "1. Display orders\n2. Create order\n0. Back\n";
        int c = readInt("Choose: ", 0, 2);
        if (c == 0) return;
        if (c == 1) listOrders(user.role == "Customer" ? user.username : "");
        if (c == 2) {
            string customer = user.role == "Customer" ? user.username : cleanField(readLine("Customer username/name: "));
            createOrder(customer);
        }
    }
}

void TeaHouseApp::createOrder(const string& customer) {
    cout << "\n========== CREATE ORDER ==========\n";
    listProducts("Tea");
    listProducts("Utensil");
    vector<pair<int,int>> chosen;
    double total = 0;
    while (true) {
        string id = readLine("Product ID (0 to finish): ");
        if (id == "0") break;
        auto it = find_if(products.begin(), products.end(), [&](const Product& p){ return p.id == id; });
        if (it == products.end()) { cout << "Product not found.\n"; continue; }
        int index = static_cast<int>(it - products.begin());
        int already = 0;
        for (const auto& item : chosen) if (item.first == index) already += item.second;
        int qty = readInt("Quantity: ", 1, 1000000);
        if (already + qty > it->stock) { cout << "Insufficient stock. Available: " << it->stock - already << "\n"; continue; }
        chosen.push_back({index,qty});
        total += it->price * qty;
        cout << "Added " << qty << " x " << it->name << ". Current total: " << money(total) << "\n";
    }
    if (chosen.empty()) { cout << "Order cancelled: no items selected.\n"; return; }
    cout << "Confirm and mark as PAID? (Y/N): ";
    string confirm = readLine("");
    if (confirm != "Y" && confirm != "y") { cout << "Order cancelled.\n"; return; }

    string itemData;
    for (size_t i = 0; i < chosen.size(); ++i) {
        Product& p = products[chosen[i].first];
        if (i) itemData += ",";
        itemData += p.id + ":" + to_string(chosen[i].second) + ":" + to_string(p.price)
                    + ":" + cleanItemName(p.name);
        p.stock -= chosen[i].second;
    }
    vector<string> ids; for (const auto& o : orders) ids.push_back(o.id);
    string id = nextId('O', ids);
    orders.push_back({id,cleanField(customer),itemData,"Paid",total});
    saveProducts();
    saveOrders();
    cout << "\nORDER / INVOICE CREATED\n";
    cout << "Invoice ID: " << id << "\nCustomer: " << customer
         << "\nTotal paid: " << money(total) << "\n";
}

void TeaHouseApp::listOrders(const string& customerFilter) const {
    cout << "\nORDER LIST\n";
    printLine();
    cout << left << setw(10) << "Order ID" << setw(22) << "Customer"
         << setw(20) << "Total" << "Status\n";
    printLine();
    bool found = false;
    for (const auto& o : orders) {
        if (!customerFilter.empty() && o.customer != customerFilter) continue;
        found = true;
        cout << left << setw(10) << o.id << setw(22) << o.customer
             << setw(20) << money(o.total) << o.status << "\n";
    }
    if (!found) cout << "No orders found.\n";
    printLine();
}

void TeaHouseApp::viewInvoices(const User& user) {
    string filter = user.role == "Customer" ? user.username : "";
    listOrders(filter);
    string id = readLine("Enter invoice/order ID to view details (Enter to return): ");
    if (id.empty()) return;
    for (const auto& o : orders) {
        if (o.id == id && (filter.empty() || o.customer == filter)) {
            cout << "\n========== INVOICE ==========\n";
            cout << "Invoice: " << o.id << "\nCustomer: " << o.customer
                 << "\nStatus: " << o.status << "\n";
            stringstream ss(o.items); string token;
            while (getline(ss, token, ',')) {
                vector<string> parts = split(token, ':');
                if (parts.size() < 2) continue;
                string pid = parts[0];
                int qty = 0;
                double soldPrice = 0;
                try {
                    qty = stoi(parts[1]);
                    if (parts.size() >= 3) soldPrice = stod(parts[2]);
                } catch (...) { continue; }
                string productName = pid;
                if (parts.size() >= 4) productName = parts[3];
                else {
                    for (const auto& p : products) {
                        if (p.id == pid) { productName = p.name; break; }
                    }
                }
                if (soldPrice <= 0) {
                    for (const auto& p : products)
                        if (p.id == pid) { soldPrice = p.price; break; }
                }
                cout << productName << " x " << qty << " @ " << money(soldPrice)
                     << " = " << money(soldPrice * qty) << "\n";
            }
            cout << "TOTAL: " << money(o.total) << "\n";
            return;
        }
    }
    cout << "Invoice not found or access denied.\n";
}

void TeaHouseApp::manageSessions(const User& user) {
    while (true) {
        cout << "\n========== TEA SESSION MANAGEMENT ==========\n"
             << "1. View sessions\n2. Add session\n3. Delete session\n0. Back\n";
        int c = readInt("Choose: ", 0, 3);
        if (c == 0) return;
        if (c == 1) listSessions();
        if (c == 2) addSession(user);
        if (c == 3) deleteSession(user);
    }
}

void TeaHouseApp::addSession(const User& user) {
    string title = cleanField(readLine("Session title: "));
    string date = readLine("Date (YYYY-MM-DD): ");
    string time = readLine("Start time (HH:MM): ");
    string desc = cleanField(readLine("Description: "));
    if (title.empty() || !validDate(date) || !validTime(time)) {
        cout << "Invalid session information.\n";
        return;
    }
    vector<string> ids; for (const auto& s : sessions) ids.push_back(s.id);
    string id = nextId('S', ids);
    sessions.push_back({id,user.username,title,date,time,desc});
    saveSessions();
    cout << "Session added. ID: " << id << "\n";
}

void TeaHouseApp::listSessions(const string& artistFilter) const {
    cout << "\nTEA SESSIONS\n";
    printLine();
    for (const auto& s : sessions) {
        if (!artistFilter.empty() && s.artist != artistFilter) continue;
        cout << s.id << " | " << s.title << " | Artist: " << s.artist
             << " | " << s.date << " " << s.startTime
             << " | " << s.description << "\n";
    }
    printLine();
}

void TeaHouseApp::deleteSession(const User& user) {
    string id = readLine("Session ID to delete: ");
    auto it = remove_if(sessions.begin(), sessions.end(), [&](const TeaSession& s) {
        return s.id == id && (user.role == "Owner" || s.artist == user.username);
    });
    if (it == sessions.end()) { cout << "Session not found or permission denied.\n"; return; }
    sessions.erase(it, sessions.end());
    saveSessions();
    cout << "Session deleted.\n";
}

void TeaHouseApp::revenueReport() {
    double roomRevenue = 0, salesRevenue = 0;
    int completedBookings = 0, paidOrders = 0;
    for (const auto& b : bookings) if (b.status == "Completed") {
        int hours = (toMinutes(b.endTime) - toMinutes(b.startTime) + 59) / 60;
        roomRevenue += hours * b.hourlyPrice;
        ++completedBookings;
    }
    for (const auto& o : orders) if (o.status == "Paid") {
        salesRevenue += o.total;
        ++paidOrders;
    }
    cout << "\n========== REVENUE REPORT ==========\n";
    cout << "Completed room bookings: " << completedBookings << "\n";
    cout << "Room revenue: " << money(roomRevenue) << "\n";
    cout << "Paid orders: " << paidOrders << "\n";
    cout << "Product revenue: " << money(salesRevenue) << "\n";
    printLine();
    cout << "TOTAL REVENUE: " << money(roomRevenue + salesRevenue) << "\n";
    printLine();
}
