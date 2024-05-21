#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <conio.h>

using namespace std;

struct member { //STRUCTURE DECLARATION FOR MEMBERS
    string name;
    string phoneNum;
    double topUpValue = 0.0;
    int membershipPoint = 0;
    string registrationNum;
};

struct Meal { //STRUCTURE DECLARATION FOR MEAL
    string name;
    double price = 0.0;
    int quantity = 0;
    bool isSet = false;
};

const int MAX_REVENUE_ENTRIES = 999;

void welcomemsg();
void registration();
void exitlogo();
double topup();
void topupOptions();
int checkMembershipPoint();
void displayMenu();
double orderFood(Meal orederedMeals[], int& numOrderedMeals);
bool validation(string& registrationNum);
void invlogin();
void storeRevenue(double totalBill);
void storeOrderedMeals(const Meal orderedMeals[], int numOrderedMeals);
bool login();
void displayAdminsMenu();
double calculateTotalRevenue();
double calculateAverageRevenue();
void sortRevenueFile();
void bubbleSort(double arr[], int n);
int individualSales(const string& mealName);
string getMealName();
int totalSales();
double calculateTax(double totalBill);

int main() {
    int choice, choice1;
    double totalBill = 0.0;
    Meal orderedMeals[15];
    int numOrderedMeals = 0;
    bool orderCompleted = false;
    bool isTakeAway = false;
    bool isDineIn = false;

    do {
        system("cls");
        welcomemsg();
        cin >> choice;
        switch (choice) {
        case 0:
            system("cls");
            exitlogo();
            exit(0);
            break;
        case 1:
            system("cls");
            srand(time(NULL));
            registration();
            cout << "Return to Main Menu? (1 to return, 0 to exit): ";
            cin >> choice1;
            if (choice1 == 0) {
                system("cls");
                exitlogo();
                exit(0);
            }
            else {
                system("cls");
                break;
            }
        case 2:///THIS IS THE START OF FOOD SELECTION / FOOD ORDERING / PAYMENT MODULE
            system("cls");
            do {//FOOR ORDERING MODULE HERE, STORES ORDERED FOOD AND QUANTITY THEN CALCULATE PRICE
                system("cls");
                displayMenu();
                double mealPrice = orderFood(orderedMeals, numOrderedMeals);
                totalBill += mealPrice;
                cout << "Would you like to add another item? (0 to finish Ordering, Other Number to continue adding): ";
                int choice;
                cin >> choice;
                system("cls");
                if (choice == 0) {
                    break;
                }
            } while (true);

            int paymentChoice; //USER SELECTS TAKE AWAY OR DINE IN, SURCHAGE DIFFERS FOR DIFFERENT OPTIONS
            cout << "Select an option:" << endl;
            cout << "1. Take Away (+RM2 for packaging)" << endl;
            cout << "2. Dine In (10% service charge)" << endl;
            cout << "0. Abort Order and Exit" << endl;
            do {
                cout << "Enter your choice: ";
                cin >> paymentChoice;
            } while (paymentChoice != 1 && paymentChoice != 2 && paymentChoice != 0);

            switch (paymentChoice) {
            case 1:
                isTakeAway = true;
                break;
            case 2:
                isDineIn = true;
                break;
            case 0:
                exitlogo();
                exit(0);
                break;
            default:
                cout << "Invalid choice. Please select a valid option." << endl;
                break;
            }
            system("cls");
            cout << "You have completed your order. What would you like to do next?" << endl;
            while (!orderCompleted) { //LET USER MODIFY THEIR ORDER. OR PAY FOR THEIR MEALS
                cout << endl;
                cout << "1. Add/Subtract Quantity" << endl;
                cout << "2. Clear Order and Exit" << endl;
                cout << "3. Proceed to Payment" << endl;
                cout << "Enter your choice: ";
                int choice;
                cin >> choice;
                switch (choice) {
                case 1: {
                    system("cls");
                    cout << "Ordered Items:" << endl;
                    const int itemNum = 4;
                    const int itemColumnWidth = 80;
                    const int quantityColumnWidth = 10;
                    const int priceColumnWidth = 8;
                    cout << left << setw(itemNum) << "NO";
                    cout << left << setw(itemColumnWidth) << "ITEM ORDERED";
                    cout << setw(quantityColumnWidth) << "QUANTITY";
                    cout << setw(priceColumnWidth) << "UNIT PRICE (RM)" << endl;
                    for (int i = 0; i < numOrderedMeals; i++) { //DISPLAYED ORDERED ITEMS AND QUANTITY
                        cout << left << setw(itemNum) << int(i) + 1;
                        cout << left << setw(itemColumnWidth) << orderedMeals[i].name;
                        cout << left << setw(quantityColumnWidth) << orderedMeals[i].quantity;
                        cout << left << setw(priceColumnWidth) << fixed << setprecision(2) << orderedMeals[i].price << endl;
                    }
                    cout << endl << "Enter the item number to update quantity: ";
                    int itemNumber;
                    cin >> itemNumber;

                    if (itemNumber >= 1 && itemNumber <= numOrderedMeals) { // UPDATE ITEM QUANTITY
                        cout << "Enter the new quantity: ";
                        int newQuantity;
                        cin >> newQuantity;

                        if (newQuantity <= 0) { // IF 0, ITEM IS REMOVED
                            totalBill -= orderedMeals[itemNumber - 1].price * orderedMeals[itemNumber - 1].quantity;
                            numOrderedMeals--;
                            for (int i = itemNumber - 1; i < numOrderedMeals; i++) {
                                orderedMeals[i] = orderedMeals[i + 1];
                            }
                            system("cls");
                            cout << "Item removed from the order." << endl << endl;
                        }
                        else { // QUANTITY UPDATED 
                            totalBill -= orderedMeals[itemNumber - 1].price * orderedMeals[itemNumber - 1].quantity;
                            orderedMeals[itemNumber - 1].quantity = newQuantity;
                            totalBill += orderedMeals[itemNumber - 1].price * newQuantity;
                            system("cls");
                            cout << "Quantity updated successfully." << endl << endl;
                        }
                    }
                    else {
                        cout << "Invalid item number. Please choose a valid item." << endl;
                    }
                    cout << "Purchased Items:" << endl; // FINAL ORDER LIST IS DISPLAYED AFTER MODIFY
                    cout << left << setw(itemNum) << "NO";
                    cout << left << setw(itemColumnWidth) << "ITEM ORDERED";
                    cout << setw(quantityColumnWidth) << "QUANTITY";
                    cout << setw(priceColumnWidth) << "UNIT PRICE (RM)" << endl;
                    for (int i = 0; i < numOrderedMeals; i++) {
                        cout << left << setw(itemNum) << int(i) + 1;
                        cout << left << setw(itemColumnWidth) << orderedMeals[i].name;
                        cout << left << setw(quantityColumnWidth) << orderedMeals[i].quantity;
                        cout << left << setw(priceColumnWidth) << fixed << setprecision(2) << orderedMeals[i].price << endl;

                    }
                    cout << "Total Bill Before Tax: RM " << totalBill << endl;
                    int continueChoice;
                    cout << "Press any number continue, Press 0 to Exit: ";
                    cin >> continueChoice;
                    system("cls");
                    if (continueChoice == 0) {
                        orderCompleted = true; // Exit the loop
                        system("cls");
                        exitlogo();
                        exit(0);
                    }
                    break;
                }

                case 2: { // TO EXIT THE PROGRAM AND CANCEL ORDER
                    numOrderedMeals = 0;
                    totalBill = 0.0;
                    cout << "Your order has been cleared." << endl;
                    orderCompleted = true;
                    system("cls");
                    exitlogo();
                    exit(0);
                    break;
                }
                case 3: { //PAYMENT MODULE STARTS HERE 
                    system("cls");
                    string registrationNum;
                    cout << "Enter Member Card Number (Case Sensitive): ";
                    cin >> registrationNum;
                    bool isRegistrationValid = validation(registrationNum); // CHECK IF REGISTRATION NUM EXISTS
                    validation(registrationNum);
                    if (!isRegistrationValid) {
                        system("cls");
                        cout << "Member Card Number Not Found!" << endl;
                        invlogin();
                        system("pause");
                        exit(0);
                    }
                    string filename = "members.txt";
                    ifstream inFile(filename);
                    if (!inFile.is_open()) {
                        cout << "Unable to open the file for reading." << endl;
                        return 0;
                    }
                    string line;
                    int lineNumber = 0, lineToReplace = 0;
                    double balance = 0.0, rebate;
                    int points = 0, pointsEarned = 0;
                    bool transaction = false, redeem = false;

                    while (getline(inFile, line)) {
                        lineNumber++;
                        if (line.find(registrationNum) != string::npos) {
                            getline(inFile, line); // Read balance
                            lineNumber++;
                            balance = stod(line);
                            getline(inFile, line); // Read points
                            points = stoi(line);
                            system("cls");
                            cout << "Current Balance: RM " << fixed << setprecision(2) << balance << endl;
                            cout << "Membership Points Balance: " << points << endl << endl;
                            break;
                        }
                    }
                    inFile.close();
                    cout << "Ordered Items:" << endl;
                    cout << "============================================================================================" << endl;
                    for (int i = 0; i < numOrderedMeals; i++) {
                        cout << i + 1 << ". " << orderedMeals[i].name << " x" << orderedMeals[i].quantity
                            << " - RM " << orderedMeals[i].price << " each" << endl;
                    }
                    cout << "============================================================================================" << endl;
                    cout << "Total Bill Before Charges : RM " << totalBill << endl;
                    double totalBill1 = totalBill + 2, totalBill2 = totalBill * 1.1;
                    if (balance >= totalBill1 || balance >= totalBill2) { //PAYMENT WITHOUT NEEDING TO TOP UP
                        if (isTakeAway) {
                            totalBill += 2;
                        }
                        else {
                            double tax = calculateTax(totalBill);
                            totalBill += tax;
                        }
                        char usePoints = ' ';
                        cout << "Total Bill After Charges : RM " << totalBill << endl;
                        if (points >= 50) { // POINTS REDEMPTION
                            cout << "The Minimum Value to Redeem Membership Points is 50 Points" << endl;
                            cout << "Do you want to redeem your membership points? (Y/N): ";
                            cin >> usePoints;
                            if (usePoints == 'y' || usePoints == 'Y') {
                                int pointsToRedeem = points / 50; // Calculate how many 50-point units can be redeemed
                                double rebate = pointsToRedeem * 0.5; // Calculate the rebate based on 50-point units
                                cout << "Rebate: RM " << fixed << setprecision(2) << rebate << endl;
                                totalBill -= rebate; // Deduct the rebate from the total bill
                                points -= pointsToRedeem * 50; // Deduct the redeemed points
                                balance -= totalBill; // Deduct totalBill from balance
                                cout << "Remaining total after using points: RM " << fixed << setprecision(2) << totalBill << endl;
                                cout << "Transaction Successful" << endl;
                                cout << "Balance after transaction: RM " << fixed << setprecision(2) << balance << endl;
                                transaction = true;
                                redeem = true;
                            }
                            else {
                                system("pause");
                                balance -= totalBill; // Deduct totalBill from balance
                                cout << "Transaction Successful" << endl;
                                cout << "Balance after transaction: RM " << fixed << setprecision(2) << balance << endl;
                                transaction = true;
                            }
                        }
                        else {
                            cout << "Not enough points to redeem." << endl;
                            balance -= totalBill; // Deduct totalBill from balance
                            cout << "Transaction Successful" << endl;
                            cout << "Balance after transaction: RM " << fixed << setprecision(2) << balance << endl;
                            transaction = true;
                        }
                    }
                    else if (balance <= totalBill1 || balance <= totalBill2) {
                        cout << "Insufficent Balance" << endl;
                        while (getline(inFile, line)) {
                            lineNumber++;
                            if (line.find(registrationNum) != string::npos) {
                                getline(inFile, line); // Read balance
                                lineNumber++;
                                balance = stod(line); // Store the balance for this user
                                system("cls");
                                cout << "Current Balance: RM " << fixed << setprecision(2) << balance << endl;
                                break; // Exit the loop once the correct balance is found
                            }
                        }
                        double currentTax;
                        inFile.close();
                        if (isDineIn) {
                            currentTax = totalBill * 0.1;
                        }
                        else if (isTakeAway) {
                            currentTax = 2;
                        }
                        cout << "Current Balance: RM " << balance << endl << endl;
                        cout << "Subtotal: RM " << fixed << setprecision(2) << totalBill << endl;
                        cout << "Charges: RM " << fixed << setprecision(2) << currentTax << endl;
                        cout << "Total Amount : RM " << fixed << setprecision(2) << totalBill + currentTax << endl << endl;
                        cout << "Amount Due: RM " << fixed << setprecision(2) << (totalBill + currentTax) - balance << endl;
                        cout << "Press 0 to exit, Press other number to top-up: " << endl;
                        int selection = -1;
                        cin >> selection;
                        if (selection == 0) {
                            system("cls");
                            exitlogo();
                            exit(0);
                            break;
                        }
                        else {
                            topupOptions();
                            int topupamount;
                            do {
                                cout << "Select top-up amount (1, 2, 3, 4, PRESS 0 TO EXIT): ";
                                cin >> topupamount;
                                switch (topupamount) {
                                case 0:
                                    system("cls");
                                    cout << "Balance after top-up: RM " << fixed << setprecision(2) << balance << endl;
                                    break;
                                case 1:
                                    balance += 4.50;
                                    cout << "Current balance: RM " << fixed << setprecision(2) << balance << endl;
                                    break;
                                case 2:
                                    balance += 9.50;
                                    cout << "Current balance: RM " << fixed << setprecision(2) << balance << endl;
                                    break;
                                case 3:
                                    balance += 29.50;
                                    cout << "Current balance: RM " << fixed << setprecision(2) << balance << endl;
                                    break;
                                case 4:
                                    balance += 49.50;
                                    cout << "Current balance: RM " << fixed << setprecision(2) << balance << endl;
                                    break;
                                default:
                                    cout << "Invalid input! Re-enter choice." << endl;
                                }
                            } while (topupamount != 0);

                            ifstream inFile(filename);
                            ofstream temp("temp.txt");

                            if (!inFile.is_open()) {
                                cout << "Error: Unable to open input file." << endl;
                                return 0.0;
                            }

                            string currentLine;

                            while (getline(inFile, currentLine)) {
                                if (currentLine.find(registrationNum) != string::npos) {
                                    // Replace the balance and points for this member
                                    temp << currentLine << endl; // Write the line with the member's ID
                                    getline(inFile, currentLine); // Read the old balance line
                                    temp << fixed << setprecision(2) << balance << endl; // Write the updated balance
                                    getline(inFile, currentLine); // Read the old points line
                                    temp << points << endl; // Write the updated points
                                }
                                else {
                                    temp << currentLine << endl; // Write other lines as is
                                }
                            }

                            inFile.close();
                            temp.close();

                            if (remove(filename.c_str()) != 0) {
                                cout << "Error: Unable to delete original file." << endl;
                                return 0.0;
                            }

                            if (rename("temp.txt", filename.c_str()) != 0) {
                                cout << "Error: Unable to rename temporary file." << endl;
                                return 0.0;
                            }

                            cout << "Balance updated successfully!" << endl;
                        }
                        break;
                    }
                    pointsEarned = int(totalBill); //WHEN PAYMENT IS COMPLETE
                    points += pointsEarned;
                    cout << "Thank you for your order!" << endl;
                    if (transaction == true) {
                        storeRevenue(totalBill);
                        storeOrderedMeals(orderedMeals, numOrderedMeals);
                        lineToReplace = 0;
                        ifstream inFile(filename);
                        ofstream temp("temp.txt");

                        if (!inFile.is_open()) {
                            cout << "Error: Unable to open input file." << endl;
                            return 0.0;
                        }

                        while (getline(inFile, line)) {
                            lineToReplace++;
                            if (lineToReplace == lineNumber) {
                                temp << fixed << setprecision(2) << balance << endl; // Write the updated balance
                                getline(inFile, line);
                                temp << points << endl;
                            }
                            else {
                                temp << line << endl; // Write other lines as is
                            }
                        }

                        inFile.close();
                        temp.close();

                        if (remove(filename.c_str()) != 0) {
                            cout << "Error: Unable to delete original file." << endl;
                            return 0.0;
                        }

                        if (rename("temp.txt", filename.c_str()) != 0) {
                            cout << "Error: Unable to rename temporary file." << endl;
                            return 0.0;
                        }
                        system("pause");
                        system("cls");
                        exitlogo();
                        exit(0);
                    }
                    break;
                }
                default:
                    cout << "Invalid choice. Please select a valid option." << endl;
                }
            }
            break;
        case 3: //TOPUP MODULE 
            system("cls");
            topup();
            cout << "Return to Main Menu? (1 to return, 0 to exit): ";
            cin >> choice1;
            if (choice1 == 0) {
                system("cls");
                exitlogo();
                exit(0);
            }
            else {
                system("cls");
                break;
            }
        case 4: // CHECK MEMBERSHIP POINTS
            system("cls");
            checkMembershipPoint();
            cout << "Return to Main Menu? (1 to return, 0 to exit): ";
            cin >> choice1;
            if (choice1 == 0) {
                system("cls");
                exitlogo();
                exit(0);
            }
            else {
                system("cls");
                break;
            }
            break;
        case 5: // THIS PART IS FOR BUSINESS OWNER TO CHECK THEIR STORE PERFORMANCE (ADVANCE FEATURES)
            int selection, selection1, selection2, selection3, Sales, mealSelection, indSales;
            double ttlRevenue, avgRevenue;
            string findMeal;
            system("cls");
            cout << "THIS PART OF THE PROGRAM IS INTENDED SOLELY FOR INDIVIDUALS WITH ADMINISTRATIVE ACCESS." << endl;
            cout << "PLEASE LOGIN TO CONTINUE (PS* PASSWORD AND USERNAME IS STORED IN A TEXT FILE INSIDE THE ASSIGNMENT FOLDER)" << endl;
            login();
            if (login) { // LOGIN
                do {
                    do {
                        system("cls");
                        displayAdminsMenu();
                        cout << "SELECT OPERATIONS (PRESS 0 TO BACK): ";
                        cin >> selection;
                        switch (selection) {
                        case 1: //REVENUE SECTION
                            system("cls");
                            cout << " _______   ________  __     __  ________  __    __  __    __  ________ " << endl;
                            cout << "|       \\ |        \\|  \\   |  \\|        \\|  \\  |  \\|  \\  |  \\|        \\" << endl;
                            cout << "| $$$$$$$\\| $$$$$$$$| $$   | $$| $$$$$$$$| $$\\ | $$| $$  | $$| $$$$$$$$" << endl;
                            cout << "| $$__| $$| $$__    | $$   | $$| $$__    | $$$\\| $$| $$  | $$| $$__" << endl;
                            cout << "| $$    $$| $$  \\    \\$$\\ /  $$| $$  \\   | $$$$\ $$| $$  | $$| $$  \\" << endl;
                            cout << "| $$$$$$$\\| $$$$$     \\$$\\  $$ | $$$$$   | $$\\$$ $$| $$  | $$| $$$$$" << endl;
                            cout << "| $$  | $$| $$_____    \\$$ $$  | $$_____ | $$ \\$$$$| $$__/ $$| $$_____" << endl;
                            cout << "| $$  | $$| $$     \\    \\$$$   | $$     \\| $$  \\$$$ \\$$    $$| $$     \\" << endl;
                            cout << " \\$$   \\$$ \\$$$$$$$$     \\$     \\$$$$$$$$ \\$$   \\$$  \\$$$$$$  \\$$$$$$$$" << endl;
                            cout << "[1]. SHOW SUM OF REVENUE." << endl;
                            cout << "[2]. SHOW AVERAGE REVENUE." << endl;
                            cout << "[3]. SORT REVENUE ASCENDINGLY." << endl;
                            cout << "SELECT OPERATIONS (PRESS 0 TO BACK): ";
                            cin >> selection1;
                            switch (selection1) {
                            case 1: // CALCULATE TOTAL REVENUE OF THE STORE
                                system("cls");
                                ttlRevenue = calculateTotalRevenue();
                                cout << "TOTAL REVENUE: RM " << ttlRevenue << endl;
                                system("pause");
                                system("cls");
                                break;
                            case 2: //CALCULATE AVERAGE REVENUE OF THE STORE
                                system("cls");
                                avgRevenue = calculateAverageRevenue();
                                cout << "AVERAGE REVENUE: RM " << fixed << setprecision(2) << avgRevenue << endl;
                                system("pause");
                                system("cls");
                                break;
                            case 3: //SORT THE REVENUE FILE ASCENDINGLY
                                system("cls");
                                sortRevenueFile();
                                system("pause");
                                break;
                            case 0:
                                break;

                            default:
                                system("cls");
                                cout << "INVALID SELECTION! LOG-IN AGAIN!";
                                exitlogo();
                                system("pause");
                                exit(0);
                            }
                            break;
                        case 2: //SALES SECTION
                            system("cls");
                            cout << "  ______    ______   __        ________   ______  " << endl;
                            cout << " /      \\  /      \\ |  \\      |        \\ /      \\ " << endl;
                            cout << "|  $$$$$$\\|  $$$$$$\\| $$      | $$$$$$$$|  $$$$$$\\ " << endl;
                            cout << "| $$___\\$$| $$__| $$| $$      | $$__    | $$___\\$$" << endl;
                            cout << " \\$$    \\ | $$    $$| $$      | $$  \\    \\$$    \\ " << endl;
                            cout << " _\\$$$$$$\\| $$$$$$$$| $$      | $$$$$    _\\$$$$$$\\" << endl;
                            cout << "|  \\__| $$| $$  | $$| $$_____ | $$_____ |  \\__| $$" << endl;
                            cout << " \\$$    $$| $$  | $$| $$     \\| $$     \\ \\$$    $$" << endl;
                            cout << "  \\$$$$$$  \\$$   \\$$ \\$$$$$$$$ \\$$$$$$$$  \\$$$$$$ " << endl;
                            cout << "[1]. CALCULATE TOTAL SANDWICHES SOLD." << endl;
                            cout << "[2]. INDIVIDUAL SANDWICH SALES." << endl;
                            cout << "ENTER YOUR OPERATION (PRESS 0 TO BACK): ";
                            cin >> selection2;
                            switch (selection2) {
                            case 1://CALCULATE TOTAL PRODUCTS SOLD (SUM OF ALL PRODUCT SOLD)
                                system("cls");
                                Sales = totalSales();
                                cout << Sales << " SANDWICHES ARE SOLD!" << endl;
                                system("pause");
                                system("cls");
                                break;
                            case 2: //CALCULATE THE INDIVIDUAL PRODUCT SALES
                                system("cls");
                                findMeal = getMealName();
                                cout << "TO GET SALES FOR SET MEALS, PRESS 1! OTHERWISE PRESS ANY NUMBER.";
                                cin >> selection3;
                                if (selection3 == 1) {
                                    findMeal = findMeal + " Set.";
                                }
                                indSales = individualSales(findMeal);
                                cout << "TOTAL SALES FOR " << findMeal << " : " << indSales << endl;
                                system("pause");
                                system("cls");
                                break;
                            case 0:
                                break;
                            default:
                                system("cls");
                                cout << "INVALID SELECTION! LOG-IN AGAIN!";
                                exitlogo();
                                system("pause");
                                system("cls");
                                exit(0);
                            }
                            break;
                        case 0:
                            break;
                        default:
                            system("cls");
                            cout << "INVALID SELECTION! PLEASE LOG-IN AGAIN!";
                        }
                    } while (selection != 0);
                } while (selection != 0);
                break;
            }
            else {
                invlogin();
                system("pause");
                break;
            }
            break;
        }

    } while (choice != 0);
    return 0;
}

void welcomemsg() { // DISPLAY LOGO AND SELECTION FOR USER
    cout << "                    _.---._" << endl;
    cout << "                _.-~       ~-._" << endl;
    cout << "            _.-~               ~-._" << endl;
    cout << "        _.-~                       ~---._" << endl;
    cout << "    _.-~                                 ~\\" << endl;
    cout << " .-~                                    _/;" << endl;
    cout << " :-._                               _.-~ ./" << endl;
    cout << "}-._~-._                   _..__.-~ _.-~)" << endl;
    cout << "`-._~-._~-._              / .__..--~_.-~" << endl;
    cout << "    ~-._~-._\\.        _.-~_/ _..--~~" << endl;
    cout << "        ~-. \\`--...--~_.-~/~~" << endl;
    cout << "            \\.`--...--~_.-~" << endl;
    cout << "              ~-..----~" << endl;
    cout << "  _____  ____  __ __   ___   __ __  ____   __ __       _____  ____  ____   ___    __    __  ____    __  __ __ " << endl;
    cout << " / ___/ /    ||  |  | /   \\ |  |  ||    \\ |  |  |     / ___/ /    ||    \\ |   \\  |  |__|  ||    |  /  ]|  |  |" << endl;
    cout << "(   \\_ |  o  ||  |  ||     ||  |  ||  D  )|  |  |    (   \\_ |  o  ||  _  ||    \\ |  |  |  | |  |  /  / |  |  |" << endl;
    cout << " \\__  ||     ||  |  ||  O  ||  |  ||    / |  ~  |     \\__  ||     ||  |  ||  D  ||  |  |  | |  | /  /  |  _  |" << endl;
    cout << " /  \\ ||  _  ||  :  ||     ||  :  ||    \\ |___, |     /  \\ ||  _  ||  |  ||     ||  `  '  | |  |/   \\_ |  |  |" << endl;
    cout << " \\    ||  |  | \\   / |     ||     ||  .  \\|     |     \\    ||  |  ||  |  ||     | \\      /  |  |\\     ||  |  |" << endl;
    cout << "  \\___||__|__|  \\_/   \\___/  \\__,_||__|\\_||____/       \\___||__|__||__|__||_____|  \\_/\\_/  |____|\\____||__|__|" << endl;
    cout << "Welcome to Savoury Sandwich! Get your Best Sandwich Here!" << endl;
    cout << "[1]. Customer Registration." << endl;
    cout << "[2]. Food Service and Selection." << endl;
    cout << "[3]. Membership Card Top-up. " << endl;
    cout << "[4]. Membership Points." << endl;
    cout << "[5]. Administration Features." << endl;
    cout << "Please select your operation (Press 0 to exit): ";
}

void registration() { // REGISTRATION MODULE
    cout << " ____    ___  ____ ____ ___________ ____   ____ ______ ____ ___  ____  " << endl;
    cout << "|    \\  /  _]/    |    / ___/      |    \\ /    |      |    /   \\|    \\" << endl;
    cout << "|  D  )/  [_|   __||  (   \\_|      |  D  )  o  |      ||  |     |  _  |" << endl;
    cout << "|    /|    _]  |  ||  |\\__  |_|  |_|    /|     |_|  |_||  |  O  |  |  |" << endl;
    cout << "|    \\|   [_|  |_ ||  |/  \\ | |  | |    \\|  _  | |  |  |  |     |  |  |" << endl;
    cout << "|  .  \\     |     ||  |\\    | |  | |  .  \\  |  | |  |  |  |     |  |  |" << endl;
    cout << "|__|\\_|_____|___,_|____|\\___| |__| |__|\\_|__|__| |__| |____\\___/|__|__|" << endl;
    member newMember;
    cout << "Enter Customer Name: ";
    cin.ignore();
    getline(cin, newMember.name);

    if (newMember.name.empty() || std::all_of(newMember.name.begin(), newMember.name.end(), ::isspace)) { // CHECK IF THE NAME IS EMPTY OR ONLY CONTAINS WHITESPACE CHARACTER
        cout << "Invalid name. Please enter a valid name." << endl;
        return;
    }

    do {
        cout << "Enter Contact Number: ";
        cin >> newMember.phoneNum;

        if (newMember.phoneNum.length() < 10 || !std::all_of(newMember.phoneNum.begin(), newMember.phoneNum.end(), ::isdigit)) { //CHECK IF THE PHONE NUMBER CONSISTS OF 10 DIGITS OR ONLY CONTAINS DIGIT
            cout << "Invalid contact number. Please enter a 10-digit valid number." << endl;
        }
    } while (newMember.phoneNum.length() < 10 || !std::all_of(newMember.phoneNum.begin(), newMember.phoneNum.end(), ::isdigit));

    int num = rand() % (1000000 - 1) + 1; // GENERATE RANDOM NUMBER
    int digits;
    string memberCard;

    if (num <= 99999) { // ADD 0 INFRONT OF NUMBERS WHEN IT DONT HAVE 6 DIGITS
        string numStr = to_string(num);
        digits = numStr.length();

        switch (digits) {
        case 1:
            memberCard = "ABC00000";
            break;
        case 2:
            memberCard = "ABC0000";
            break;
        case 3:
            memberCard = "ABC000";
            break;
        case 4:
            memberCard = "ABC00";
            break;
        case 5:
            memberCard = "ABC0";
            break;
        }
    }
    else {
        memberCard = "ABC";
    }

    memberCard += to_string(num);
    newMember.registrationNum = memberCard;
    newMember.topUpValue = 0.0;
    newMember.membershipPoint = 0;

    ofstream outFile("members.txt", ios::app);

    if (outFile.is_open()) { // WRITE THE DATA TO FILES
        outFile << newMember.registrationNum << " ; " << newMember.name << " ; " << newMember.phoneNum << "\n"
            << newMember.topUpValue << "\n" << newMember.membershipPoint << "\n\n";
        outFile.close();
        cout << "Member card number is: " << newMember.registrationNum << endl;
        cout << "Ask the customer to note down their Member Card Number for future usage." << endl;
    }
    else {
        cout << "Error: Unable to open the file for writing." << endl;
    }
}

void exitlogo() { //USE WHEN SYSTEM IS TERMINATED
    cout << "  _____  ___   ___      __ __  ___  __ __       ____  ____  ____ ____ ____  " << endl;
    cout << " / ___/ /  _] /  _]    |  |  |/   \\|  |  |     /    |/    |/    |    |    \\ " << endl;
    cout << "(   \\_ /  [_ /  [_     |  |  |     |  |  |    |  o  |   __|  o  ||  ||  _  |" << endl;
    cout << " \\__  |    _]    _]    |  ~  |  O  |  |  |    |     |  |  |     ||  ||  |  |" << endl;
    cout << " /  \\ |   [_|   [_     |___, |     |  :  |    |  _  |  |_ |  _  ||  ||  |  |" << endl;
    cout << " \\    |     |     |    |     |     |     |    |  |  |     |  |  ||  ||  |  |" << endl;
    cout << "  \\___|_____|_____|    |____/ \\___/ \\__,_|    |__|__|___,_|__|__|____|__|__|" << endl;
}

double topup() { // TOP UP MODULE
    cout << " ______  ___  ____  __ __ ____  " << endl;
    cout << "|      |/   \\|    \\|  |  |    \\ " << endl;
    cout << "|      |     |  o  )  |  |  o  )" << endl;
    cout << "|_|  |_|  O  |   _/|  |  |   _/ " << endl;
    cout << "  |  | |     |  |  |  :  |  |   " << endl;
    cout << "  |  | |     |  |  |     |  |   " << endl;
    cout << "  |__|  \\___/|__|   \\__,_|__|   " << endl;

    string registrationNum;
    cout << "Enter Member Card Number (Case Sensitive): ";
    cin >> registrationNum;
    bool isRegistrationValid = validation(registrationNum); // VALIDATES REGISTRATIONNUM
    validation(registrationNum);
    if (!isRegistrationValid) {
        system("cls");
        invlogin();
        exit(0);
    }
    string filename = "members.txt";
    ifstream inFile(filename);

    if (!inFile.is_open()) {
        cout << "Unable to open the file for reading." << endl;
        return 0.0;
    }

    string line;
    int lineNumber = 0;
    int topupamount = 0;
    double balance = 0.0;

    while (getline(inFile, line)) { //READ LINE BY LINE
        lineNumber++;
        if (line.find(registrationNum) != string::npos) { // IF REGISTRATION NUMBER FOUND
            getline(inFile, line); // GET NEXT LINE
            lineNumber++;
            balance = stod(line); // STORE BALANCE
            system("cls");
            cout << "Current Balance: RM " << fixed << setprecision(2) << balance << endl << endl;
            break;
        }
    }

    topupOptions();

    do {
        cout << "Select top up amount (1, 2, 3, 4, PRESS 0 TO EXIT): ";
        cin >> topupamount;
        switch (topupamount) {
        case 0:
            system("cls");
            cout << "Balance after top up: RM " << fixed << setprecision(2) << balance << endl;
            break;
        case 1:
            balance += 4.50;
            cout << "Current balance: RM " << fixed << setprecision(2) << balance << endl;
            break;
        case 2:
            balance += 9.50;
            cout << "Current balance: RM " << fixed << setprecision(2) << balance << endl;
            break;
        case 3:
            balance += 29.50;
            cout << "Current balance: RM " << fixed << setprecision(2) << balance << endl;
            break;
        case 4:
            balance += 49.50;
            cout << "Current balance: RM " << fixed << setprecision(2) << balance << endl;
            break;
        default:
            cout << "Invalid input! Re-enter choice." << endl;
        }
    } while (topupamount != 0);

    string inputFile = "members.txt";
    string tempFile = "temp.txt";
    if (!inFile.is_open()) {
        cout << "Error: Unable to open input file." << endl;
        return 0.0;
    }
    ofstream temp(tempFile);
    if (!temp.is_open()) {
        cout << "Error: Unable to open temporary file." << endl;
        return 0.0;
    }
    int lineToReplace = 0;

    inFile.clear(); // CLEAR THE ERROR FLAG
    inFile.seekg(0, ios::beg); // MOVE FILE POINTER TO BEGINNING
    while (getline(inFile, line)) {
        lineToReplace++;
        if (lineToReplace == lineNumber) { // IF LINE TO REPLACE IS = LINE NUMBER, REPLACE THE LINE
            temp << balance << endl;
        }
        else {
            temp << line << endl; // WRITE BACK CURRENT LINE IF NOT EQUAL
        }
    }
    inFile.close();
    temp.close(); // CLOSE AND RENAME FILES
    if (remove(inputFile.c_str()) != 0) {
        cout << "Error: Unable to delete original file." << endl;
        return 0.0;  // Return 0.0 to indicate an error.
    }

    if (rename(tempFile.c_str(), inputFile.c_str()) != 0) {
        cout << "Error: Unable to rename temporary file." << endl;
        return 0.0;  // Return 0.0 to indicate an error.
    }

    cout << "Balance updated successfully!" << endl;

    return balance;  // Return the updated balance.
}

void topupOptions() { // DISPLAY TOPUP OPTIONS
    cout << "Top-up amount:" << endl;
    cout << "[1] RM 5" << endl;
    cout << "[2] RM 10" << endl;
    cout << "[3] RM 30" << endl;
    cout << "[4] RM 50" << endl;
    cout << "Note* RM0.50 will be charged for each top-up" << endl;
}

int checkMembershipPoint() {
    cout << " ___ ___   ___ ___ ___ ____    ___ ____   _______ __ ____ ____       ____   ___ ____ ____  ______  _____" << endl;
    cout << "|   |   | /  _]   |   |    \\  /  _]    \\ / ___/  |  |    |    \\     |    \\ /   \\    |    \\|      |/ ___/" << endl;
    cout << "| _   _ |/  [_| _   _ |  o  )/  [_|  D  |   \\_|  |  ||  ||  o  )    |  o  )     |  ||  _  |      (   \\_ " << endl;
    cout << "|  \\_/  |    _]  \\_/  |     |    _]    / \\__  |  _  ||  ||   _/     |   _/|  O  |  ||  |  |_|  |_|\__  |" << endl;
    cout << "|   |   |   [_|   |   |  O  |   [_|    \\ /  \\ |  |  ||  ||  |       |  |  |     |  ||  |  | |  |  /  \\ |" << endl;
    cout << "|   |   |     |   |   |     |     |  .  \\\\    |  |  ||  ||  |       |  |  |     |  ||  |  | |  |  \\    |" << endl;
    cout << "|___|___|_____|___|___|_____|_____|__|\\_| \\___|__|__|____|__|       |__|   \\___/____|__|__| |__|   \\___|" << endl;
    string registrationNum;
    cout << "Enter Member Card Number (Case Sensitive): ";
    cin >> registrationNum;
    bool isRegistrationValid = validation(registrationNum); // VALIDATE REGISTRATION
    validation(registrationNum);
    if (!isRegistrationValid) {
        system("cls");
        invlogin();
        exit(0);
    }
    int points = -1;
    string filename = "members.txt";
    ifstream inFile(filename);

    if (!inFile.is_open()) {
        cout << "Unable to open the file for reading." << endl;
        return points;
    }

    string line;
    int lineNumber = 0;

    while (getline(inFile, line)) {
        lineNumber++;
        if (line.find(registrationNum) != string::npos) { //WHEN FOUND THE MEMBERCARD ID, READ NEXT 2 LINES AND THEN EXTRACT POINTS
            getline(inFile, line);
            getline(inFile, line);
            points = stoi(line);
            system("cls");
            cout << "Available points: " << points << endl;
        }
    }
    inFile.close();
    return points;
}

void displayMenu() { // DISPLAY MENU
    cout << " ___ ___   ___ ____  __ __ " << endl;
    cout << "|   |   | /  _]    \\|  |  |" << endl;
    cout << "| _   _ |/  [_|  _  |  |  |" << endl;
    cout << "|  \\_/  |    _]  |  |  |  |" << endl;
    cout << "|   |   |   [_|  |  |  :  |" << endl;
    cout << "|   |   |     |  |  |     |" << endl;
    cout << "|___|___|_____|__|__|\\__,_|" << endl;
    cout << ",================================================================================================," << endl;
    cout << "|              Sandwiches                                                        Unit Price (RM) |" << endl;
    cout << "|[1]. Tuna Swiss Cheese Sandwich with Onions, Olives and Avocadoes                   19.50       |" << endl;
    cout << "|[2]. Beef Parmesan Sandwich with Pickles, Cucumbers and Sprouts                     14.00       |" << endl;
    cout << "|[3]. Mixed Seafood Parmesan Sandwich with Lettuce, Peppers and Olives               21.50       |" << endl;
    cout << "|[4]. Turkey Provolons Sandwich with Onions, Tomatoes and Peppers                    17.50       |" << endl;
    cout << "|[5]. Ham Gouda Sandwich with Sprouts, Olives and Tomatoes                           14.00       |" << endl;
    cout << "|[6]. Egg Mayo Sandwich                                                              7.00        |" << endl;
    cout << "|[7]. Chicken Slices Ricotta Sandwich with Sprouts, Lettuces and Avocadoes           16.00       |" << endl;
    cout << "|[8]. Sausages Mascarpone Sandwich with Cucumbers, Pickles, and Lettuce              16.00       |" << endl;
    cout << "|[9]. Bacon Gouda sandwich with Peppers, Olives and Cucumbers                        17.00       |" << endl;
    cout << "|[10].King Crab Swiss Cheese Sandwich with Pickles, Tomatoes and Peppers             29.00       |" << endl;
    cout << "|[11].Mini Lobster Swiss Cheese Sandwich with Peppers, Onions and Lettuce            29.00       |" << endl;
    cout << "|[12].Salmon Parmesan Sandwich with Onions, Lettuce and Sprouts                      29.00       |" << endl;
    cout << "|[13].Salami Mascarpone Sandwich with Cucumbers, Pickles and Onions                  29.00       |" << endl;
    cout << "|[14].Duck Breast Camembert Sandwich with Onions, Cucumbers and Lettuce              26.00       |" << endl;
    cout << "|[15].Bacon and Eggs Cream Cheese Sandwich                                           26.00       |" << endl;
    cout << "'================================================================================================'" << endl;
}

double orderFood(Meal orderedMeals[], int& numOrderedMeals) {
    double mealPrice = 0.0;
    int choice;

    cout << "Please choose a meal by entering its number: "; // ORDER FOOD FROM MENU
    cin >> choice;

    if (choice >= 1 && choice <= 15) {
        Meal meal;
        switch (choice) { //MATCHING MEAL CODES WITH SWITCH STATEMENTS
        case 1:
            meal.name = "Tuna Swiss Cheese Sandwich with Onions, Olives and Avocadoes";
            meal.price = 19.50;
            break;
        case 2:
            meal.name = "Beef Parmesan Sandwich with Pickles, Cucumbers and Sprouts";
            meal.price = 14.00;
            break;
        case 3:
            meal.name = "Mixed Seafood Parmesan Sandwich with Lettuce, Peppers and Olives";
            meal.price = 21.50;
            break;
        case 4:
            meal.name = "Turkey Provolons Sandwich with Onions, Tomatoes and Peppers";
            meal.price = 17.50;
            break;
        case 5:
            meal.name = "Ham Gouda Sandwich with Sprouts, Olives and Tomatoes";
            meal.price = 14.00;
            break;
        case 6:
            meal.name = "Egg Mayo Sandwich";
            meal.price = 7.00;
            break;
        case 7:
            meal.name = "Chicken Slices Ricotta Sandwich with Sprouts, Lettuces and Avocadoes";
            meal.price = 16.00;
            break;
        case 8:
            meal.name = "Sausages Mascarpone Sandwich with Cucumbers, Pickles, and Lettuce";
            meal.price = 16.00;
            break;
        case 9:
            meal.name = "Bacon Gouda Sandwich with Peppers, Olives and Cucumbers";
            meal.price = 17.00;
            break;
        case 10:
            meal.name = "King Crab Swiss Cheese Sandwich with Pickles, Tomatoes and Peppers";
            meal.price = 29.00;
            break;
        case 11:
            meal.name = "Mini Lobster Swiss Cheese Sandwich with Peppers, Onions and Lettuce";
            meal.price = 29.00;
            break;
        case 12:
            meal.name = "Salmon Parmesan Sandwich with Onions, Lettuce and Sprouts";
            meal.price = 29.00;
            break;
        case 13:
            meal.name = "Salami Mascarpone Sandwich with Cucumbers, Pickles and Onions";
            meal.price = 29.00;
            break;
        case 14:
            meal.name = "Duck Breast Camembert Sandwich with Onions, Cucumbers and Lettuce";
            meal.price = 26.00;
            break;
        case 15:
            meal.name = "Bacon and Eggs Cream Cheese Sandwich";
            meal.price = 26.00;
            break;
        default:
            cout << "Invalid choice. Please select a valid option." << endl;
        }
        cout << "How many servings would you like to order? "; // GET QUANTITY
        cin >> meal.quantity;
        cout << "Select an option:" << endl; // ASK IF USER WANTS SET OR ALA-CARTE
        cout << "1. Set Meal (+RM3, includes coke and fries)" << endl;
        cout << "2. Ala-Carte " << endl;
        cout << "Press 1 for Set Meal / Other Number for Ala-Carte" << endl;
        cout << "Your option :";
        cin >> choice;
        if (choice == 1) {
            meal.isSet = true;
            meal.price += 3.0; // Add RM3 for the set
            cout << "Added " << meal.quantity << " servings of " << meal.name << " with a set of coke and fries." << endl;
            meal.name += " Set.";
        }
        else {
            meal.isSet = false;
            cout << "Added " << meal.quantity << " servings of " << meal.name << " to your order." << endl;
        }

        orderedMeals[numOrderedMeals++] = meal; // Store the ordered meal
        mealPrice = meal.price * meal.quantity;
    }
    else {
        cout << "Invalid meal number. Please choose a valid meal." << endl;
    }

    return mealPrice;
}

bool validation(string& registrationNum) {
    string filename = "members.txt";
    ifstream inFile(filename);

    if (!inFile.is_open()) {
        cout << "Unable to open the file for reading." << endl;
        return false;
    }

    string word;
    while (inFile >> word) { // IF FILES FOUND SAME MEMBERCARD NUMBER AS INPUT PARAMETER, RETURN TRUE
        if (word == registrationNum) {
            inFile.close();
            return true;
        }
    }

    inFile.close();
    return false;
}

void invlogin() { //DISPLAY ACCESS DENIED
    cout << "  __    ___   ___  ____  ____  ____        ____  ____  __ _   __   ____  ____ \n";
    cout << " / _\\  / __) / __)(  __)/ ___)/ ___)      (    \\(  __)(  ( \\ (  ) (  __)(    \\ \n";
    cout << "/    \\( (__ ( (__  ) _) \\___ \\\\___ \\       ) D ( ) _) /    /  )(   ) _)  ) D (\n";
    cout << "\\_/\\_/ \\___) \\___)(____)(____/(____/      (____/(____)\\_)__) (__) (____)(____/\n";

}

void storeRevenue(double totalBill) {
    ofstream outFile("revenue.txt", ios::app); // Open the file in append mode

    if (!outFile.is_open()) {
        cout << "Unable to open the revenue file for writing." << endl;
        return;
    }

    outFile << totalBill << endl; // Write the totalBill value to the file followed by a newline character

    outFile.close(); // Close the file
}

void storeOrderedMeals(const Meal orderedMeals[], int numOrderedMeals) {
    ofstream outFile("ordered_meals.txt", ios::app);  // Open the file in append mode

    if (!outFile.is_open()) {
        cout << "Error: Unable to open the file for writing." << endl;
        return;
    }
    for (int i = 0; i < numOrderedMeals; i++) { //STORE ORDERED MEALS TO FILE 
        for (int j = 0; j < orderedMeals[i].quantity; j++) {
            outFile << orderedMeals[i].name << endl;
        }
    }
    outFile.close();
}

void displayAdminsMenu() {
    cout << "  ______   _______   __       __  ______  __    __ " << endl;
    cout << " /      \\ |       \\ |  \\     /  \\|      \\|  \\  |  \\" << endl;
    cout << "|  $$$$$$\\| $$$$$$$\\| $$\\   /  $$ \\$$$$$$| $$\\ | $$" << endl;
    cout << "| $$__| $$| $$  | $$| $$$\\ /  $$$  | $$  | $$$\\| $$" << endl;
    cout << "| $$    $$| $$  | $$| $$$$\   $$$$  | $$  | $$$$\\ $$" << endl;
    cout << "| $$$$$$$$| $$  | $$| $$\\$$ $$ $$  | $$  | $$\\$$ $$" << endl;
    cout << "| $$  | $$| $$__/ $$| $$ \\$$$| $$ _| $$_ | $$ \\$$$$" << endl;
    cout << "| $$  | $$| $$    $$| $$  \\$ | $$|   $$ \\| $$  \\$$$" << endl;
    cout << " \\$$   \\$$ \\$$$$$$$  \\$$      \\$$ \\$$$$$$ \\$$   \\$$" << endl;
    cout << "[1]. STORE REVENUE" << endl;
    cout << "[2]. SET MEALS SALES" << endl;
}

bool login() {
    const string cusername = "ADMIN";
    const string abort = "BYE";
    string username;
    string password; // Assuming the password is a string
    const string cpassword = "0000"; // Password as a string
    bool loggedIn = false;
    cout << " __         ______    ______   ______  __    __ " << endl;
    cout << "|  \\       /      \\  /      \\ |      \\|  \\  |  \\" << endl;
    cout << "| $$      |  $$$$$$\\|  $$$$$$\\ \\$$$$$$| $$\\ | $$" << endl;
    cout << "| $$      | $$  | $$| $$ __\\$$  | $$  | $$$\\| $$" << endl;
    cout << "| $$      | $$  | $$| $$|    \\  | $$  | $$$$\ $$" << endl;
    cout << "| $$      | $$  | $$| $$ \\$$$$  | $$  | $$\\$$ $$" << endl;
    cout << "| $$_____ | $$__/ $$| $$__| $$ _| $$_ | $$ \\$$$$" << endl;
    cout << "| $$     \\ \\$$    $$ \\$$    $$|   $$ \\| $$  \\$$$" << endl;
    cout << " \\$$$$$$$$  \\$$$$$$   \\$$$$$$  \\$$$$$$ \\$$   \\$$" << endl;
    while (!loggedIn) {
        cout << "ENTER USERNAME (ENTER \"BYE\" TO ABORT): ";
        cin >> username;

        if (username == cusername) {
            cout << "ENTER PASSWORD: ";
            cin >> password;

            if (password == cpassword) {
                loggedIn = true;
            }
            else {
                loggedIn = false;
            }
        }
        else if (username == abort) {
            system("cls");
            exitlogo();
            exit(0);
        }
        else {
            loggedIn = false;
        }
        cout << "INCORRECT PASSWORD OR USERNAME, PLEASE RE-ENTER" << endl;

    }

    cout << "Login successful!" << endl;
    return true;
}

double calculateTotalRevenue() {
    ifstream inFile("revenue.txt"); // Open the file for reading
    if (!inFile.is_open()) {
        cout << "Unable to open the revenue file for reading." << endl;
        return 0.0; // Return 0.0 to indicate an error
    }
    double totalRevenue = 0.0;
    double value;
    while (inFile >> value) {
        totalRevenue += value;
    }
    inFile.close(); // Close the file
    return totalRevenue;
}

double calculateAverageRevenue() {
    ifstream inFile("revenue.txt"); // Open the file for reading
    if (!inFile.is_open()) {
        cout << "Unable to open the revenue file for reading." << endl;
        return 0.0; // Return 0.0 to indicate an error
    }
    double totalRevenue = 0.0;
    double value;
    int count = 0;

    while (inFile >> value) {
        totalRevenue += value;
        count++;
    }
    inFile.close(); // Close the file
    if (count == 0) {
        return 0.0; // Return 0.0 if there are no values in the file to avoid division by zero
    }
    double averageRevenue = totalRevenue / count;
    return averageRevenue;
}

void bubbleSort(double arr[], int n) { //BUBBLE SORT ALGORITHM
    bool swapped;
    do {
        swapped = false;
        for (int i = 1; i < n; i++) {
            if (arr[i - 1] > arr[i]) { //COMPARE ARRAY CONTENTS AND SWAP IF A GRETER ELEMENT IS FOUND
                swap(arr[i - 1], arr[i]);
                swapped = true;
            }
        }
    } while (swapped);
}

void sortRevenueFile() {
    ifstream inFile("revenue.txt"); // Open the file for reading
    if (!inFile.is_open()) {
        cout << "Unable to open the revenue file for reading." << endl;
        return;
    }
    double revenueData[MAX_REVENUE_ENTRIES];
    double value;
    int count = 0;
    while (inFile >> value) {
        if (count < MAX_REVENUE_ENTRIES) {
            revenueData[count++] = value;
        }
        else {
            cout << "Warning: Maximum number of revenue entries exceeded. Ignoring additional entries." << endl;
        }
    }
    inFile.close(); // Close the file

    if (count == 0) {
        cout << "No revenue data to sort." << endl;
        return;
    }
    bubbleSort(revenueData, count);
    ofstream outFile("revenue_sorted.txt"); // Open a new file for writing the sorted data
    if (!outFile.is_open()) {
        cout << "Unable to open the sorted revenue file for writing." << endl;
        return;
    }
    for (int i = 0; i < count; i++) {
        outFile << revenueData[i] << endl;
    }
    outFile.close(); // Close the sorted file
    cout << "Revenue data has been sorted in ascending order and saved to 'revenue_sorted.txt'." << endl;
}

int totalSales() {
    string filename = "ordered_meals.txt";
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Error: Unable to open the file for reading." << endl;
        return -1;
    }

    int lineNum = 0;
    string line;
    while (getline(inFile, line)) {
        lineNum++; // COUNT TOTAL SALES OF THE STORE
    }
    inFile.close();
    return lineNum;
}

int individualSales(const string& mealName) { // TAKE MEALNAME AS PARAMETER ANAD FIND THE OCCURENCE USING LINEAR SEARCH
    string filename = "ordered_meals.txt";
    ifstream inFile(filename);
    int count = 0;
    string line;

    if (!inFile.is_open()) {
        cout << "Error: Unable to open the file for reading." << endl;
        return -1;
    }

    while (getline(inFile, line)) {
        if (line == mealName) {
            count++;
        }
    }

    inFile.close();
    return count;
}

string getMealName() {
    displayMenu();
    int mealCode;
    cout << "ENTER SET NUMBER: ";
    cin >> mealCode;
    switch (mealCode) { // MATCH THE SWITCH STATEMENT WITH MENU AND RETURN MEALNAME
    case 1: return "Tuna Swiss Cheese Sandwich with Onions, Olives and Avocadoes";
    case 2: return "Beef Parmesan Sandwich with Pickles, Cucumbers and Sprouts";
    case 3: return "Mixed Seafood Parmesan Sandwich with Lettuce, Peppers and Olives";
    case 4: return "Turkey Provolons Sandwich with Onions, Tomatoes and Peppers";
    case 5: return "Ham Gouda Sandwich with Sprouts, Olives and Tomatoes";
    case 6: return "Egg Mayo Sandwich";
    case 7: return "Chicken Slices Ricotta Sandwich with Sprouts, Lettuces and Avocadoes";
    case 8: return "Sausages Mascarpone Sandwich with Cucumbers, Pickles, and Lettuce";
    case 9: return "Bacon Gouda sandwich with Peppers, Olives and Cucumbers";
    case 10: return "King Crab Swiss Cheese Sandwich with Pickles, Tomatoes and Peppers";
    case 11: return "Mini Lobster Swiss Cheese Sandwich with Peppers, Onions and Lettuce";
    case 12: return "Salmon Parmesan Sandwich with Onions, Lettuce and Sprouts";
    case 13: return "Salami Mascarpone Sandwich with Cucumbers, Pickles and Onions";
    case 14: return "Duck Breast Camembert Sandwich with Onions, Cucumbers and Lettuce";
    case 15: return "Bacon and Eggs Cream Cheese Sandwich";
    default: return "Invalid item code";
    }
}

double calculateTax(double totalBill) { //GET TOTAL BILL AS PARAMETER THEN CALCULATE TAX AND RETURN TAX
    const double taxRate = 0.10;
    return totalBill * taxRate;
}