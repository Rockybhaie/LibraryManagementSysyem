#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <ctime>
#include <stdexcept>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <memory>
#include <algorithm>
#include <numeric>
#include <functional>
#include <set>
#include <map>
#include <unordered_map>

using namespace std;

class AllExceptions : public exception {
    string message;
public:
    AllExceptions(const string& msg) : message(msg) {}

    virtual const char* what() const noexcept {
        return message.c_str();
    }
};

class Book {
    int ISBN;
    string title;
    string author;
    string genre;
    int noOfcopies;
    double rating;
    int timesIssued;

public:
    Book() : rating(0), timesIssued(0) {}

    Book(int ISBN, string title, string author, string genre, int noOfcopies)
        : ISBN(ISBN), title(title), author(author), genre(genre), noOfcopies(noOfcopies), rating(0), timesIssued(0) {}

    int getISBN() const {
        return ISBN;
    }

    string getGenre() const {
        return genre;
    }

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    int getNoofCopies() const {
        return noOfcopies;
    }

    double getRating() const {
        return rating;
    }

    int getTimesIssued() const {
        return timesIssued;
    }

    void Noofcopiesincrement() {
        noOfcopies += 1;
    }

    void Noofcopiesdecrement() {
        if (noOfcopies > 0) {
            noOfcopies -= 1;
        }
    }

    void addRating(double newRating) {
        if (timesIssued > 0) {
            rating = (rating * (timesIssued - 1) + newRating) / timesIssued;
        } else {
            rating = newRating;
        }
    }

    void incrementTimesIssued() {
        timesIssued++;
    }

    friend ostream& operator<<(ostream& os, const Book& obj);
};

ostream& operator<<(ostream& os, const Book& obj) {
    os << "ISBN: " << obj.ISBN << endl
       << "Title: " << obj.title << endl
       << "Author: " << obj.author << endl
       << "Genre: " << obj.genre << endl
       << "Number of copies: " << obj.noOfcopies << endl
       << "Rating: " << fixed << setprecision(2) << obj.rating << endl
       << "Times Issued: " << obj.timesIssued << endl << endl;
    return os;
}

class Library {
protected:
    string LibraryName;
    string LibraryAddress;
    vector<Book> Librarymain;

public:
    Library() {
        LibraryName = "JINAAH";
        LibraryAddress = "21st street";
    }

    void LoadLibrary(const string& filename) {
        ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            throw AllExceptions("Error opening file: " + filename);
        }

        int ISBN, noOfcopies, timesIssued;
        string title, author, genre;
        double rating;
        while (inputFile >> ISBN >> title >> author >> genre >> noOfcopies >> rating >> timesIssued) {
            Book book(ISBN, title, author, genre, noOfcopies);
            book.addRating(rating);
            for (int i = 0; i < timesIssued; ++i) {
                book.incrementTimesIssued();
            }
            Librarymain.push_back(book);
        }

        inputFile.close();
    }

    void UpdateLibrarytotextfile(const string& filename) {
        ofstream outputFile(filename);

        if (!outputFile.is_open()) {
            throw AllExceptions("Error opening file for writing: " + filename);
        }

        for (const auto& book : Librarymain) {
            outputFile << book.getISBN() << " "
                       << book.getTitle() << " "
                       << book.getAuthor() << " "
                       << book.getGenre() << " "
                       << book.getNoofCopies() << " "
                       << book.getRating() << " "
                       << book.getTimesIssued() << endl;
        }
        outputFile.close();
    }

    void printLibrary() const {
        for (const auto& book : Librarymain) {
            cout << book;
        }
    }

    void addBookintolibray(const Book& book) {
        auto lambda = [&book](const Book& obj) {
            return obj.getISBN() == book.getISBN();
        };
        auto it = find_if(Librarymain.begin(), Librarymain.end(), lambda);
        if (it != Librarymain.end()) {
            throw AllExceptions("Book with given ISBN already exists.");
        } 

        Librarymain.push_back(book);
    }

    void removeBookFromlibrary(int isbn) {
        auto lambda = [&isbn](const Book& obj) {
            return obj.getISBN() == isbn;
        };
        auto it = find_if(Librarymain.begin(), Librarymain.end(), lambda);
        if (it != Librarymain.end()) {
            Librarymain.erase(it);
        } else {
            throw AllExceptions("Book with given ISBN not found.");
        }
    }

    void SearchinLibrary(function<bool(const Book&)> func) const {
        vector<Book> Filtered;

        copy_if(Librarymain.begin(), Librarymain.end(), back_inserter(Filtered), func);

        for (const Book& book : Filtered) {
            cout << book;
        }

        Filtered.clear();
    }

    Book& FindBookinLibISBN(function<bool(Book&)> func) {
        auto it = find_if(Librarymain.begin(), Librarymain.end(), func);

        if (it != Librarymain.end()) {
            return *it;
        } else {
            throw AllExceptions("Book with entered ISBN not found in library");
        }
    }

    void AvalaibleBooks() const {
        auto lam = [](const Book& obj) -> bool {
            return obj.getNoofCopies() > 0;
        };

        vector<Book> Filtered;

        copy_if(Librarymain.begin(), Librarymain.end(), back_inserter(Filtered), lam);

        for (const Book& book : Filtered) {
            cout << book;
        }

        Filtered.clear();
    }

    vector<Book> getTop10Books() const {
        vector<Book> sortedBooks = Librarymain;
        sort(sortedBooks.begin(), sortedBooks.end(), [](const Book& a, const Book& b) {
            return a.getTimesIssued() > b.getTimesIssued();
        });
        if (sortedBooks.size() > 10) {
            sortedBooks.resize(10);
        }
        return sortedBooks;
    }

    void importBooksFromSecondary(Library& secondaryLib, int isbn) {
        try {
            auto lam = [&isbn](Book& obj) -> bool {
                return obj.getISBN() == isbn;
            };

            Book& book = secondaryLib.FindBookinLibISBN(lam);
            addBookintolibray(book);
            secondaryLib.removeBookFromlibrary(isbn);
            cout << "Book imported successfully from secondary library." << endl;
        } catch (const AllExceptions& e) {
            cerr << "IMPORT ERROR: " << e.what() << endl;
        }
    }
};

class Admin {
    int adminId;
    string adminName;
    int passcode;
    Library lib;
    Library secondaryLib;

public:
    Admin() {}
    Admin(int id, string name, int c) : adminId(id), adminName(name), passcode(c) {}

    void setlib(Library& lib) {
        this->lib = lib; 
    }

    void setSecondaryLib(Library& secondaryLib) {
        this->secondaryLib = secondaryLib; 
    }

    int getAdminid() const {
        return adminId;
    }

    string getAdminname() const {
        return adminName;
    }

    int getpasscode() const {
        return passcode;
    }

    void addBook() {
        int ISBN;
        string title;
        string author;
        string genre;
        int noOfcopies;
        cout << "ENTER DETAILS OF THE BOOK YOU WANT TO ADD TO THE LIBRARY: " << endl;
        cout << "Enter the ISBN of the Book: ";
        cin >> ISBN;
        cout << "Enter the title of the Book: ";
        cin >> title;
        cout << "Enter the author of the Book: ";
        cin >> author;
        cout << "Enter the genre of the Book: ";
        cin >> genre;
        cout << "Enter the number of copies of the Book: ";
        cin >> noOfcopies;

        try {
            Book book(ISBN, title, author, genre, noOfcopies);
            lib.addBookintolibray(book);
            lib.UpdateLibrarytotextfile("D:\\Library_main.txt");
            cout << "Book added to library successfully" << endl;
        } catch (const AllExceptions& obj) {
            cerr << "ERROR ADDING BOOK: " << obj.what() << endl;
        }
    }

    void removeBook() {
        int isbn;
        cout << "Enter the ISBN number of the book you want to remove: " << endl;
        cin >> isbn;
        try {
            lib.removeBookFromlibrary(isbn);
            lib.UpdateLibrarytotextfile("D:\\Library_main.txt");
            cout << "Book with ISBN: " << isbn << " removed from library successfully" << endl;
        } catch (const AllExceptions& obj) {
            cerr << "ERROR REMOVING BOOK: " << obj.what() << endl;
        }
    }

    void printLibraryContentsUpdated() const {
        lib.printLibrary();
    }

    void SearchBook() {
        try {
            cout << "How do you want to search the book?" << endl;
            cout << "1. By ISBN" << endl;
            cout << "2. By Genre" << endl;
            cout << "3. By Title" << endl;
            cout << "4. By Author" << endl;
            int choice;
            cout << "choice: ";
            cin >> choice;
            vector<Book> Filtered;

            if (choice == 1) {
                int isbn;
                cout << "Enter the ISBN OF THE BOOK" << endl;
                cin >> isbn;

                auto lam = [&isbn](const Book& obj) -> bool {
                    return obj.getISBN() == isbn;
                };

                lib.SearchinLibrary(lam);
            } else if (choice == 2) {
                string GENRE;
                cout << "Enter the Genre" << endl;
                cin >> GENRE;

                auto lam = [&GENRE](const Book& obj) -> bool {
                    return obj.getGenre() == GENRE;
                };

                lib.SearchinLibrary(lam);
            } else if (choice == 3) {
                string name;
                cout << "Enter the Title" << endl;
                cin >> name;

                auto lam = [&name](const Book& obj) -> bool {
                    return obj.getTitle() == name;
                };

                lib.SearchinLibrary(lam);
            } else if (choice == 4) {
                string authorr;
                cout << "Enter the Author" << endl;
                cin >> authorr;

                auto lam = [&authorr](const Book& obj) -> bool {
                    return obj.getAuthor() == authorr;
                };

                lib.SearchinLibrary(lam);
            } else {
                throw AllExceptions("Invalid choice.");
            }
        } catch (const AllExceptions& obj) {
            cerr << "SEARCHBOOK ERROR: " << obj.what() << endl;
        }
    }

    void viewAvalaibleBooks() const {
        lib.AvalaibleBooks();
    }

    void importBooks() {
        int isbn;
        cout << "Enter the ISBN of the book to import from secondary library: ";
        cin >> isbn;
        lib.importBooksFromSecondary(secondaryLib, isbn);
        lib.UpdateLibrarytotextfile("D:\\Library_main.txt");
        secondaryLib.UpdateLibrarytotextfile("D:\\Secondary_Library.txt");
    }

    void printTop10Books() const {
        vector<Book> top10Books = lib.getTop10Books();
        cout << "Top 10 Most Popular Books:" << endl;
        for (const auto& book : top10Books) {
            cout << book;
        }
    }
};

class User {
    int UserId;
    string UserName;
    int passcode;
    Library lib;
    vector<Book> Wishlist;
    vector<Book> inventory;
    unordered_map<int, time_t> issueDates;

public:
    User() {}

    User(int id, string name, int code) : UserId(id), UserName(name), passcode(code) {}

    void setlib(Library& lib) {
        this->lib = lib; 
    }

    int getuserid() const {
        return UserId;
    }

    string getusername() const {
        return UserName;
    }

    int getpasscode() const {
        return passcode;
    }

    void printLibraryContentsUpdated() const {
        lib.printLibrary();
    }

    void wishlist() {
        try {
            cout << "Enter the ISBN of book you want to add to your Wishlist: ";
            int isbn;
            cin >> isbn;
            auto lam = [&isbn](const Book& obj) -> bool {
                return obj.getISBN() == isbn;
            };
            Book book = lib.FindBookinLibISBN(lam);

            Wishlist.push_back(book);
            cout << "Book added successfully" << endl;
            cout << endl;

            cout << "WISHLIST:" << endl;

            for (const auto& it : Wishlist) {
                cout << it << endl;
            }
        } catch (const AllExceptions& obj) {
            cerr << "WISHLIST ERROR: " << obj.what() << endl;
        }
    }

    void SearchBook() {
        try {
            cout << "How do you want to search the book?" << endl;
            cout << "1. By ISBN" << endl;
            cout << "2. By Genre" << endl;
            cout << "3. By Title" << endl;
            cout << "4. By Author" << endl;
            int choice;
            cout << "choice: ";
            cin >> choice;
            vector<Book> Filtered;

            if (choice == 1) {
                int isbn;
                cout << "Enter the ISBN OF THE BOOK" << endl;
                cin >> isbn;

                auto lam = [&isbn](const Book& obj) -> bool {
                    return obj.getISBN() == isbn;
                };

                lib.SearchinLibrary(lam);
            } else if (choice == 2) {
                string GENRE;
                cout << "Enter the Genre" << endl;
                cin >> GENRE;

                auto lam = [&GENRE](const Book& obj) -> bool {
                    return obj.getGenre() == GENRE;
                };

                lib.SearchinLibrary(lam);
            } else if (choice == 3) {
                string name;
                cout << "Enter the Title" << endl;
                cin >> name;

                auto lam = [&name](const Book& obj) -> bool {
                    return obj.getTitle() == name;
                };

                lib.SearchinLibrary(lam);
            } else if (choice == 4) {
                string authorr;
                cout << "Enter the Author" << endl;
                cin >> authorr;

                auto lam = [&authorr](const Book& obj) -> bool {
                    return obj.getAuthor() == authorr;
                };

                lib.SearchinLibrary(lam);
            } else {
                throw AllExceptions("Invalid choice.");
            }
        } catch (const AllExceptions& obj) {
            cerr << "SEARCHBOOK ERROR: " << obj.what() << endl;
        }
    }

    void viewAvalaibleBooks() const {
        lib.AvalaibleBooks();
    }

    void IssueBook() {
        try {
            cout << "Enter the ISBN of the book you want to issue: ";
            int isbn;
            cin >> isbn;

            auto lam = [&isbn](Book& obj) -> bool {
                return obj.getISBN() == isbn;
            };

            Book& obj = lib.FindBookinLibISBN(lam);

            if (obj.getNoofCopies() > 0) {
                time_t currentTime = time(0);
                tm* currentDate = localtime(&currentTime);

                issueDates[isbn] = currentTime;

                inventory.push_back(obj);
                cout << "You have been issued the book for 7 days." << endl;
                obj.Noofcopiesdecrement();
                obj.incrementTimesIssued();
                lib.UpdateLibrarytotextfile("D:\\Library_main.txt");
                SaveUserData();
            } else {
                cout << "The book you want is out of stock. Please check again in a few days." << endl;
            }
        } catch (const AllExceptions& obj) {
            cerr << "ISSUEBOOK ERROR: " << obj.what() << endl;
        }
    }

    void ReturnBook() {
        try {
            cout << "Enter the ISBN of the book you want to return: ";
            int isbn;
            cin >> isbn;

            auto lam = [&isbn](Book& obj) -> bool {
                return obj.getISBN() == isbn;
            };

            Book& obj = lib.FindBookinLibISBN(lam);

            if (issueDates.find(isbn) == issueDates.end()) {
                cout << "You have not issued this book." << endl;
                return;
            }

            time_t currentTime = time(0);
            tm* currentDate = localtime(&currentTime);

            double duration = difftime(currentTime, issueDates[isbn]) / (24 * 3600);

            double fine = max(duration - 7, 0.0) * 1000.0;

            if (fine > 0) {
                cout << "You returned the book " << duration << " days late." << endl;
                cout << "Your fine amount is: $" << fine << endl;
            } else {
                cout << "Book returned successfully." << endl;
            }

            inventory.erase(remove_if(inventory.begin(), inventory.end(), [&](Book& b) { return b.getISBN() == isbn; }), inventory.end());
            obj.Noofcopiesincrement();
            lib.UpdateLibrarytotextfile("D:\\Library_main.txt");
            issueDates.erase(isbn);
            SaveUserData();
            rateBook(isbn);

        } catch (const AllExceptions& obj) {
            cerr << "RETURNBOOK ERROR: " << obj.what() << endl;
        }
    }

    void SaveUserData() const {
        ofstream outFile("D:\\User_" + to_string(UserId) + "_data.txt");

        if (!outFile.is_open()) {
            throw AllExceptions("Error opening file for writing user data.");
        }

        for (const auto& book : inventory) {
            outFile << book.getISBN() << " "
                    << book.getTitle() << " "
                    << book.getAuthor() << " "
                    << book.getGenre() << " "
                    << book.getNoofCopies() << " "
                    << issueDates.at(book.getISBN()) << endl;
        }

        outFile.close();
    }

    void LoadUserData() {
        ifstream inFile("D:\\User_" + to_string(UserId) + "_data.txt");

        if (!inFile.is_open()) {
            cerr << "Error opening file for reading user data." << endl;
            return;
        }

        int ISBN, noOfcopies;
        string title, author, genre;
        time_t issueDate;
        while (inFile >> ISBN >> title >> author >> genre >> noOfcopies >> issueDate) {
            Book book(ISBN, title, author, genre, noOfcopies);
            inventory.push_back(book);
            issueDates[ISBN] = issueDate;
        }

        inFile.close();
    }

    void rateBook(int isbn) {
        try {
            auto lam = [&isbn](Book& obj) -> bool {
                return obj.getISBN() == isbn;
            };

            Book& book = lib.FindBookinLibISBN(lam);

            double rating;
            cout << "Rate the book (0.0 to 5.0): ";
            while (!(cin >> rating) || rating < 0.0 || rating > 5.0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid rating. Please enter a rating between 0.0 and 5.0: ";
            }

            book.addRating(rating);
            lib.UpdateLibrarytotextfile("D:\\Library_main.txt");
            cout << "Thank you for rating the book!" << endl;
        } catch (const AllExceptions& obj) {
            cerr << "RATING ERROR: " << obj.what() << endl;
        }
    }
};

User signUp() {
    bool c=true;
    int id;
    string Name;
    int code;
    while(c==true){
        
        cout << "Enter User ID: ";
        cin >> id;

        vector<User> userinfo;
        ifstream userFile("D:\\User_info");
        if (!userFile.is_open()) {
            cerr << "Error opening user file for reading." << endl;
        }

        int userId, passcode;
        string userName;
        while (userFile >> userId >> userName >> passcode) {
        userinfo.push_back(User(userId, userName, passcode));
        }
        userFile.close();
        bool a= false;
    

        for (const auto& user : userinfo) {
            if (user.getuserid() == id) {
                cout<<"User ID already exists. Please try again with a different ID."<<endl;
                userinfo.clear();
                a=true;
            }
        }

        if(a==false){
            c=false;
        }

    }

    cout << "Enter Username: ";
    cin >> Name;
    cout << "Enter Passcode: ";
    cin >> code;

    
    ofstream userFileOut("D:\\User_info", ios::app);
    if (!userFileOut.is_open()) {
        cerr << "Error opening user file for writing." << endl;
    }

    
    userFileOut << id << " " << Name << " " << code << endl;
    userFileOut.close();

    cout << "User signed up successfully!" << endl;

    User user(id, Name, code);
    return user;
    

    

    
}

User readUsersandcheckLogin() {
    bool c=true;

    while(c==true){
        
    
    cout << "Please enter your User ID: ";
    int id;
    while (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter your User ID: ";
    }

    cout << "Please enter your Passcode: ";
    int c;
    while (!(cin >> c)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter your Passcode: ";
    }


    vector<User> userinfo;


    ifstream userFile("D:\\User_info");
    if (!userFile.is_open()) {
        cerr << "Error opening user file for reading." << endl;
    }
    int userId, passcode;
    string userName;
    while (userFile >> userId >> userName >> passcode) {
        userinfo.push_back(User(userId, userName, passcode));
    }

    userFile.close();

    for (auto it : userinfo) {
        if (it.getuserid() == id && it.getpasscode() == c) {
            it.LoadUserData();
            return it;
        }
    }

    cerr<<"INVALID CREDENTIALS TRY AGAIN"<<endl<<endl;
    }

}

Admin readAdminandcheckLogin() {
    bool c=true;
    while(c==true){
    cout << "Please enter your Admin ID: ";
    int id;
    while (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter your Admin ID: ";
    }

    cout << "Please enter your Passcode: ";
    int c;
    while (!(cin >> c)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter your Passcode: ";
    }
    vector<Admin> admininfo;

    ifstream adminFile("D:\\Admin_info.txt");
    if (!adminFile.is_open()) {
        cerr << "Error opening admin file for reading." << endl;
    }

    int adminId, passcode;
    string adminName;
    while (adminFile >> adminId >> adminName >> passcode) {
        admininfo.push_back(Admin(adminId, adminName, passcode));
    }

    adminFile.close();

    for (auto it : admininfo) {
        if (it.getAdminid() == id && it.getpasscode() == c) {
            return it;
        }
    }

    cerr<<"INVALID CREDENTIALS TRY AGAIN"<<endl<<endl;
    }
}

void Interface() {
    Library lib;
    Library secondaryLib;
    try {
        lib.LoadLibrary("D:\\Library_main.txt");
        secondaryLib.LoadLibrary("D:\\Secondary_Library.txt");
    } catch (const AllExceptions& e) {
        cerr << "Error: " << e.what() << endl;
        return;
    }

    cout << "-----------------------------------------" << endl;
    cout << "Welcome to the Library Management System!" << endl;
    cout << "-----------------------------------------" << endl;
    bool c=true;
    while(c==true){
    cout << "Please choose your role:" << endl;
    cout << "0: Admin" << endl;
    cout << "1: User" << endl;
    
    int choice;
    while (!(cin >> choice) || (choice != 0 && choice != 1)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter 0 for Admin or 1 for User: ";
    }

    if (choice == 1) {
        User user1;

        cout << "Would you like to:" << endl;
        cout << "0: Sign up" << endl;
        cout << "1: Log in" << endl;
        
        int b;
        while (!(cin >> b) || (b != 0 && b != 1)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 0 to Sign up or 1 to Log in: ";
        }

        if (b == 0) {
            try {
                user1 = signUp();
                user1.setlib(lib);
            } catch (const AllExceptions& e) {
                cerr << "Error: " << e.what() << endl;
                return;
            }
        } else {
            

            
            user1 = readUsersandcheckLogin();
            user1.setlib(lib);
            
        }

        int a = 0;
        while (a != 7) {
            cout << "Please choose an action:" << endl;
            cout << "1: View Available Books" << endl;
            cout << "2: Search for a Book" << endl;
            cout << "3: Issue a Book" << endl;
            cout << "4: Return a Book" << endl;
            cout << "5: Add a Book to Wishlist" << endl;
            cout << "6: Logout" << endl;
            cout << "7: Exit from Library" << endl;
            
            while (!(cin >> a) || a < 1 || a > 7) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please choose a number between 1 and 6: ";
            }

            switch (a) {
                case 1:
                    user1.viewAvalaibleBooks();
                    cout<<endl;
                    break;
                case 2:
                    user1.SearchBook();
                    cout<<endl;
                    break;
                case 3:
                    user1.IssueBook();
                    cout<<endl;
                    break;
                case 4:
                    user1.ReturnBook();
                    cout<<endl;
                    break;
                case 5:
                    user1.wishlist();
                    cout<<endl;
                    break;
                case 6:
                    a=7;
                    cout<<endl;
                    break;
                case 7:
                    cout << "Thank you for using the Library Management System. Goodbye!" << endl;
                    return;
            }
        }

    } else {
        Admin admin1;
        
            admin1 = readAdminandcheckLogin();
            admin1.setlib(lib);
            admin1.setSecondaryLib(secondaryLib);
         

        int a = 0;
        while (a != 9) {
            cout << "Please choose an action:" << endl;
            cout << "1: Add a Book to Library" << endl;
            cout << "2: Remove a Book from Library" << endl;
            cout << "3: View Library Contents" << endl;
            cout << "4: Search for a Book" << endl;
            cout << "5: View Available Books" << endl;
            cout << "6: Import Books from Secondary Library" << endl;
            cout << "7: View Top 10 Books" << endl;
            cout << "8: Logout" << endl;
            cout << "9: Exit from Library" << endl;

            while (!(cin >> a) || a < 1 || a > 9) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please choose a number between 1 and 8: ";
            }

            switch (a) {
                case 1:
                    admin1.addBook();
                    cout<<endl;
                    break;
                case 2:
                    admin1.removeBook();
                    cout<<endl;
                    break;
                case 3:
                    admin1.printLibraryContentsUpdated();
                    cout<<endl;
                    break;
                case 4:
                    admin1.SearchBook();
                    cout<<endl;
                    break;
                case 5:
                    admin1.viewAvalaibleBooks();
                    cout<<endl;
                    break;
                case 6:
                    admin1.importBooks();
                    cout<<endl;
                    break;
                case 7:
                    admin1.printTop10Books();
                    cout<<endl;
                    break;
                case 8:
                    a=9;
                    cout<<endl;
                    break;
                case 9:
                    cout << "Thank you for managing the Library. Goodbye!" << endl;
                    return;
            }
        }
    }
    }
}




int main() {
    Interface();
    return 0;
}