# Tea House Management System

A C++17 command-line (CUI) application for managing a tea house.

## Features

- Login and role-based menus: Owner, Staff, Artist, Customer, Guest
- User management (Owner can add accounts)
- Room management: list, add, edit, delete
- Booking management: create, check overlapping time slots, cancel, complete
- Tea product and tea utensil management: list, add, edit, delete, stock
- Orders and invoices: create orders, calculate totals, deduct stock, view invoices
- Tea session management: artists can manage their own sessions
- Revenue report: completed room bookings + paid product orders
- Text-file persistence

## Requirements

- Windows with MinGW-w64 / MSYS2 GCC, or another C++17 compiler
- Visual Studio Code (optional)

## Files

```text
TeaHouseManagement/
|-- main.cpp
|-- TeaHouse.h
|-- TeaHouse.cpp
|-- users.txt
|-- rooms.txt
|-- products.txt
|-- bookings.txt
|-- orders.txt
|-- sessions.txt
|-- README.md
```

## Compile on Windows (PowerShell)

Open the terminal in this folder and run:

```powershell
g++ main.cpp TeaHouse.cpp -o TeaHouse -std=c++17
```

Run:

```powershell
.\TeaHouse.exe
```

## Default accounts

| Role | Username | Password |
|---|---|---|
| Owner | owner | 123456 |
| Staff | staff01 | 123456 |
| Artist | artist01 | 123456 |
| Customer | customer01 | 123456 |

If `users.txt`, `rooms.txt`, or `products.txt` is missing, the program creates sample data automatically.

## Data files

The program reads and writes the following files in its current working directory:

- `users.txt`: `id|username|password|fullName|role`
- `rooms.txt`: `id|name|capacity|hourlyPrice|status|description`
- `products.txt`: `id|name|category|unit|price|stock`
- `bookings.txt`: `id|customer|roomId|date|startTime|endTime|hourlyPrice|status`
- `orders.txt`: `id|customer|itemList|total|status` (new order items store `productId:quantity:unitPrice:name` snapshots)
- `sessions.txt`: `id|artistUsername|title|date|startTime|description`

Do not manually insert the `|` character into fields. The program replaces it with `/` in text fields.

## Business rules and scope

- Booking time uses `YYYY-MM-DD` and `HH:MM` (24-hour clock).
- A room cannot have overlapping confirmed bookings on the same date.
- Booking duration is rounded up to a whole hour for the estimate.
- A completed booking is counted as room revenue.
- Product orders are marked Paid after confirmation and stock is deducted.
- Customers can view and cancel only bookings belonging to their login username and can view their own orders/invoices.
- Staff and Owner can create bookings/orders for a customer name or username.
- Guest mode is read-only.

## Important limitations

This is a classroom project/demo, not a production payment or security system:
- Passwords are stored as plain text in `users.txt`.
- No real payment gateway is connected.
- Revenue is based on completed bookings and orders marked Paid.
- Booking date validation checks calendar validity but does not prevent booking past dates.
- Data files should be backed up before manual editing.


## Project documents

- `BAO_CAO_DU_AN.md`: Vietnamese project analysis, role requirements, OOP design, file format, scope and presentation outline.
- `TEST_CASES.md`: tested cases and additional checklist for group verification.

## Tea business sample catalog

The sample catalog follows the assignment scenario: four tea types, pot serving (about 8–10g), loose tea packs (50g, 100g, 200g, 357g tea cake, 1kg), and tea utensils. Prices for weight-based packs and utensils are sample data for demonstration and should be replaced with the group's agreed business assumptions.
