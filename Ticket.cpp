#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// Base class 'Ticket' using inheritance
class Ticket {
public:
    // Constructor overloading in the base class
    Ticket(string name, float price, int count)
        : name(name), price(price), count(count) {}

    // Virtual function for dynamic polymorphism
    virtual void display() const = 0;
    virtual void book(int row, int column) = 0;

protected:
    string name;
    float price;
    int count;
};

// Derived class 'BookSeat' using inheritance
class BookSeat : public Ticket {
public:
    // Constructor overloading in the derived class
    BookSeat(string name, float price, int count)
        : Ticket(name, price, count) {}

    void display() const override {
        cout << "Ticket: " << name << " - Rs " << price << " Available: " << count << endl;
    }

    void book(int row, int column) override {
        if (count > 0) {
            cout << "Booking seat " << row << "-" << static_cast<char>('A' + column - 1) << " for " << name << " at Rs " << price << endl;
            count--;
        } else {
            cout << "No available seats for " << name << endl;
        }
    }
};

// Base class 'Payment' using inheritance
class Payment {
public:
    // Virtual function for dynamic polymorphism
    virtual void paymentMethod() = 0;
};

// Derived class 'CardPay' using inheritance
class CardPay : public Payment {
public:
    void paymentMethod() override {
        cout << "Enter card details (CVV): ";
        string cvv, expDate;
        cin >> cvv;

        if (isValidCVV(cvv)) {
            cout << "Enter expiry date (mm/yyyy): ";
            cin >> expDate;

            if (isValidExpiryDate(expDate)) {
                cout << "Your card payment has been processed successfully." << endl;
            } else {
                cout << "Invalid expiry date format or values. Please enter the date in mm/yyyy format." << endl;
                paymentMethod(); // Ask the user to re-enter the details
            }
        } else {
            cout << "Invalid CVV entered! Please enter a 3-digit CVV number." << endl;
            paymentMethod(); // Ask the user to re-enter the CVV
        }
    }

    // Function for CVV validation
    bool isValidCVV(const string& cvv) {
        return cvv.length() == 3 && cvv.find_first_not_of("0123456789") == string::npos;
    }

    // Function for expiry date validation
    bool isValidExpiryDate(const string& expDate) {
        if (expDate.length() != 7)
            return false;

        int month, year;
        if (sscanf(expDate.c_str(), "%d/%d", &month, &year) != 2)
            return false;

        if (month < 1 || month > 12 || year < 1900)
            return false;

        return true;
    }
};

// Derived class 'UPIPay' using inheritance
class UPIPay : public Payment {
public:
    void paymentMethod() override {
        cout << "Enter UPI phone number: ";
        long long phoneNumber;
        cin >> phoneNumber;
        if (isValidMobileNumber(phoneNumber)) {
            cout << "Your UPI payment has been processed successfully." << endl;
        } else {
            cout << "Invalid phone number entered! Please enter a 10-digit mobile number." << endl;
            paymentMethod(); // Ask the user to re-enter the phone number
        }
    }

    // Function for mobile number validation
    bool isValidMobileNumber(long long phoneNumber) {
        string phoneNumberStr = to_string(phoneNumber);
        return phoneNumberStr.length() == 10;
    }
};

// Function to show seating layout
void showLayout(string movieName) {
    cout << "Seating Layout for " << movieName << ":\n";
}

// Function to generate a ticket
void generateTicket(string movieName, string showtime, vector<pair<int, int>>& bookedSeats) {
    string stars = "****************************************";

    cout << "\n" << stars << "\n";
    cout << "*  Movie: " << movieName << "\n";
    cout << "*  Showtime: " << showtime << "\n";
    cout << "*  Booked Seats: ";
    
    for (const auto& seat : bookedSeats) {
        cout << seat.first << "-" << static_cast<char>('A' + seat.second - 1) << " ";
    }
    
    cout << "\n" << stars << "\n";
}

int main() {
    vector<string> movies = { "Movie A", "Movie B", "Movie C" };
    vector<string> showtimes = { "10:00 AM", "02:30 PM", "07:00 PM" };

    BookSeat TicketsforA("Movie A", 100.0, 20);
    BookSeat TicketsforB("Movie B", 250.0, 30);
    BookSeat TicketsforC("Movie C", 350.0, 18);

    string selectedMovie;
    int choice;

    while (true) {
        cout << "\nOnline Movie Ticket Booking System\n";
        cout << "1. See Available Movies and Showtimes\n";
        cout << "2. Book Tickets and make payment\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nAvailable Movies and Showtimes:\n";
                for (int i = 0; i < movies.size(); ++i) {
                    cout << i + 1 << ". " << movies[i] << " - " << showtimes[i] << endl;
                }
                break;

            case 2:
                int ChoiceOfMovie, numOfTickets;
                cout << "Enter the movie number to book tickets: ";
                cin >> ChoiceOfMovie;

                if (ChoiceOfMovie >= 1 && ChoiceOfMovie <= movies.size()) {
                    cout << "Enter the number of tickets to book: ";
                    cin >> numOfTickets;

                    selectedMovie = movies[ChoiceOfMovie - 1];
                    showLayout(selectedMovie);
                    
                    vector<pair<int, int>> bookedSeats;
                    
                    for (int i = 0; i < numOfTickets; i++) {
                        int row, column;
                        cout << "Enter the row (1-" << numOfTickets << "): ";
                        cin >> row;

                        if (row < 1 || row > numOfTickets) {
                            cout << "Invalid row choice." << endl;
                            break;
                        }

                        cout << "Enter the column (1 for A, 2 for B, ...): ";
                        int col;
                        cin >> col;

                        if (col < 1 || col > 26) {
                            cout << "Invalid column choice." << endl;
                            break;
                        }
                        
                        bookedSeats.push_back({row, col});

                        if (selectedMovie == "Movie A") {
                            TicketsforA.book(row, col);
                        } else if (selectedMovie == "Movie B") {
                            TicketsforB.book(row, col);
                        } else if (selectedMovie == "Movie C") {
                            TicketsforC.book(row, col);
                        }
                    }

                    int paymentChoice;
                    cout << "Select Payment Method:\n";
                    cout << "1. Card\n";
                    cout << "2. UPI\n";
                    cout << "Enter your choice: ";
                    cin >> paymentChoice;

                    Payment* paymentProcess;
                    if (paymentChoice == 1) {
                        paymentProcess = new CardPay();
                    } else if (paymentChoice == 2) {
                        paymentProcess = new UPIPay();
                    } else {
                        cout << "Invalid payment choice." << endl;
                        continue;
                    }

                    paymentProcess->paymentMethod();
                    delete paymentProcess;
                    generateTicket(selectedMovie, showtimes[ChoiceOfMovie - 1], bookedSeats);
                    cout << "\nThank you for using our Online Movie Ticket Booking System!\n";
                    return 0;
                } else {
                    cout << "Invalid movie choice." << endl;
                }
                break;

            default:
                cout << "Invalid choice. Please select a valid option.\n";
                break;
        }
    }

    return 0;
}
