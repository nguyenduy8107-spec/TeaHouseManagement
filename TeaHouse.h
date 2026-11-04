#ifndef TEAHOUSE_H
#define TEAHOUSE_H

#include <string>
#include <vector>

struct User {
    std::string id, username, password, fullName, role;
};

struct Room {
    std::string id, name, description, status;
    int capacity = 0;
    double hourlyPrice = 0;
};

struct Product {
    std::string id, name, category, unit;
    double price = 0;
    int stock = 0;
};

struct Booking {
    std::string id, customer, roomId, date, startTime, endTime, status;
    double hourlyPrice = 0;
};

struct Order {
    std::string id, customer, items, status;
    double total = 0;
};

struct TeaSession {
    std::string id, artist, title, date, startTime, description;
};

class TeaHouseApp {
private:
    std::vector<User> users;
    std::vector<Room> rooms;
    std::vector<Product> products;
    std::vector<Booking> bookings;
    std::vector<Order> orders;
    std::vector<TeaSession> sessions;

    void loadAll();
    void saveUsers();
    void saveRooms();
    void saveProducts();
    void saveBookings();
    void saveOrders();
    void saveSessions();

    void mainMenu();
    void ownerMenu(const User& user);
    void staffMenu(const User& user);
    void artistMenu(const User& user);
    void customerMenu(const User& user);
    void guestMenu();

    User* authenticate();
    void manageUsers();
    void manageRooms();
    void manageProducts(const std::string& category);
    void manageBookings(const User& user);
    void manageOrders(const User& user);
    void manageSessions(const User& user);
    void viewInvoices(const User& user);
    void revenueReport();

    void addRoom();
    void editRoom();
    void deleteRoom();
    void listRooms() const;

    void addProduct(const std::string& category);
    void editProduct(const std::string& category);
    void deleteProduct(const std::string& category);
    void listProducts(const std::string& category) const;

    void createBooking(const std::string& customer);
    void listBookings(const std::string& customerFilter = "") const;
    void cancelBooking(const std::string& customerFilter = "");
    void completeBooking();

    void createOrder(const std::string& customer);
    void listOrders(const std::string& customerFilter = "") const;

    void addSession(const User& user);
    void listSessions(const std::string& artistFilter = "") const;
    void deleteSession(const User& user);

    std::string nextId(char prefix, const std::vector<std::string>& existingIds) const;
    bool roomExists(const std::string& id) const;
    bool productExists(const std::string& id) const;
    bool bookingTimeAvailable(const std::string& roomId,
                              const std::string& date,
                              const std::string& start,
                              const std::string& end) const;

public:
    TeaHouseApp();
    void run();
};

#endif
