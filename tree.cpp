#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

/*
    Each booking represents ONE hour slot for a room.
    Multi-hour classes are split into multiple Booking records.
*/
struct Booking {
    string date;      // YYMMDD  (e.g. 260102 = 2 Jan 2026)
    int hour;         // 8 - 16 (8am to 5pm)
    string room;      // e.g. C301
    string lecturer;  // Lecturer name
    string course;    // Course code
};

struct TreeNode {
    Booking info;
    TreeNode* left;
    TreeNode* right;
};

/*
    This function creates a UNIQUE KEY for every booking.
    The BST will be sorted using this key.
    Format: YYMMDD + HH + Room
    Example: 26010210C301
*/
string makeKey(const Booking& b) {
    return b.date + (b.hour < 10 ? "0" : "") + to_string(b.hour) + b.room;
}

/*
    Overloaded version to build a key directly from input.
*/
string makeKey(string date, int hour, string room) {
    return date + (hour < 10 ? "0" : "") + to_string(hour) + room;
}

bool Insert(TreeNode*& tree, Booking b) {
    if (tree == NULL) {
        tree = new TreeNode{b, NULL, NULL};
        return true;
    }

    string newKey = makeKey(b);
    string curKey = makeKey(tree->info);

    if (newKey == curKey)
        return false;
    else if (newKey < curKey)
        return Insert(tree->left, b);
    else
        return Insert(tree->right, b);
}

bool Search(TreeNode* tree, string key, Booking& result) {
    if (tree == NULL) return false;

    string curKey = makeKey(tree->info);

    if (key == curKey) {
        result = tree->info;
        return true;
    }
    else if (key < curKey)
        return Search(tree->left, key, result);
    else
        return Search(tree->right, key, result);
}

TreeNode* FindMin(TreeNode* tree) {
    while (tree->left != NULL)
        tree = tree->left;
    return tree;
}

bool Delete(TreeNode*& tree, string key) {
    if (tree == NULL) return false;

    string curKey = makeKey(tree->info);

    if (key < curKey)
        return Delete(tree->left, key);
    else if (key > curKey)
        return Delete(tree->right, key);
    else {
        // Node found
        if (tree->left == NULL && tree->right == NULL) {
            delete tree;
            tree = NULL;
        }
        else if (tree->left == NULL) {
            TreeNode* temp = tree;
            tree = tree->right;
            delete temp;
        }
        else if (tree->right == NULL) {
            TreeNode* temp = tree;
            tree = tree->left;
            delete temp;
        }
        else {
            // Two children: replace with inorder successor
            TreeNode* temp = FindMin(tree->right);
            tree->info = temp->info;
            Delete(tree->right, makeKey(temp->info));
        }
        return true;
    }
}

void Display(TreeNode* tree) {
    if (tree != NULL) {
        Display(tree->left);

        cout << "| " << setw(6) << tree->info.date
             << " | " << setw(2) << tree->info.hour << ":00"
             << " | " << setw(6) << tree->info.room
             << " | " << setw(12) << tree->info.lecturer
             << " | " << setw(10) << tree->info.course
             << " |\n";

        Display(tree->right);
    }
}

void DisplayByDate(TreeNode* tree, string date) {
    if (tree != NULL) {
        DisplayByDate(tree->left, date);

        if (tree->info.date == date) {
            cout << "| " << setw(6) << tree->info.date
                 << " | " << setw(2) << tree->info.hour << ":00"
                 << " | " << setw(6) << tree->info.room
                 << " | " << setw(12) << tree->info.lecturer
                 << " | " << setw(10) << tree->info.course
                 << " |\n";
        }

        DisplayByDate(tree->right, date);
    }
}

void DisplayByRoom(TreeNode* tree, string room) {
    if (tree != NULL) {
        DisplayByRoom(tree->left, room);

        if (tree->info.room == room) {
            cout << "| " << setw(6) << tree->info.date
                 << " | " << setw(2) << tree->info.hour << ":00"
                 << " | " << setw(6) << tree->info.room
                 << " | " << setw(12) << tree->info.lecturer
                 << " | " << setw(10) << tree->info.course
                 << " |\n";
        }

        DisplayByRoom(tree->right, room);
    }
}

/*
    Validate that date is in YYMMDD format
    and month/day are in valid range.
*/
bool validDate(string d) {
    if (d.length() != 6) return false;

    int month = stoi(d.substr(2,2));
    int day = stoi(d.substr(4,2));

    return (month >= 1 && month <= 12 && day >= 1 && day <= 31);
}

bool validHour(int h) {
    return (h >= 8 && h <= 16);
}

bool validDuration(int startHour, int duration) {
    return duration >= 1 && startHour + duration - 1 <= 16;
}

void SaveToFile(TreeNode* tree, ofstream& out) {
    if (!tree) return;
    SaveToFile(tree->left, out);
    out << tree->info.date << "," << tree->info.hour << ","
        << tree->info.room << "," << tree->info.lecturer << ","
        << tree->info.course << endl;
    SaveToFile(tree->right, out);
}

void RewriteFile(TreeNode* root) {
    ofstream out("bookings.txt");
    SaveToFile(root, out);
    out.close();
}

void LoadFromFile(TreeNode*& root) {
    ifstream in("bookings.txt");
    if (!in) return;

    Booking b;
    while (getline(in, b.date, ',')) {
        in >> b.hour; in.ignore();
        getline(in, b.room, ',');
        getline(in, b.lecturer, ',');
        getline(in, b.course);
        Insert(root, b);
    }
    in.close();
}

void menu() {
    cout << "\n=== ROOM BOOKING SYSTEM ===\n";
    cout << "1. Book Room\n";
    cout << "2. Cancel Booking\n";
    cout << "3. Search Booking\n";
    cout << "4. Display All Bookings\n";
    cout << "5. Display Schedule by Date\n";
    cout << "6. Display Schedule by Room\n";
    cout << "7. Exit\n";
    cout << "Choose: ";
}

int main() {
    TreeNode* root = NULL;
    LoadFromFile(root);

    int choice;

    do {
        menu();
        cin >> choice;

        // Book room
        if (choice == 1) {
            Booking b;
            int duration;

            do {
                cout << "Enter Date (YYMMDD): ";
                cin >> b.date;

                if (!validDate(b.date))
                    cout << "Invalid date. Please enter again.\n";

            } while (!validDate(b.date));

            do {
                cout << "Enter Start Hour (8-16): ";
                cin >> b.hour;

                if (!validHour(b.hour))
                    cout << "Invalid hour. Must be between 8 and 16.\n";

            } while (!validHour(b.hour));

            do {
                cout << "Enter Duration (hours): ";
                cin >> duration;

                if (!validDuration(b.hour, duration))
                    cout << "Error: Class exceeds 5pm.\n";

            } while (!validDuration(b.hour, duration));

            do {
                cout << "Enter Room (1-20): ";
                cin >> duration;

                if (!(b.room >= "1" && b.room <= "20"))
                    cout << "Chooese a existing room to book\n";

            } while (!(b.room >= "1" && b.room <= "20"));
            
            cin.ignore(); 
a
            cout << "Enter Lecturer: ";
            getline(cin, b.lecturer);

            cout << "Enter Course: ";
            getline(cin, b.course);

            // Check if any of the required time slots is already booked
            bool conflict = false;
            for (int i = 0; i < duration; i++) {
                Booking temp = b;
                temp.hour = b.hour + i;
                Booking dummy;
                if (Search(root, makeKey(temp), dummy)) {
                    conflict = true;
                    break;
                }
            }

            if (conflict) {
                cout << "Error: One or more time slots already booked.\n";
            } else {
                // Insert each hour into BST
                for (int i = 0; i < duration; i++) {
                    Booking temp = b;
                    temp.hour = b.hour + i;
                    Insert(root, temp);
                }
                RewriteFile(root);
                cout << "Booking successful.\n";
            }
        }

        // Cancel booking
        else if (choice == 2) {
            string date, room;
            int hour;

            cout << "Enter Date (YYMMDD): ";
            cin >> date;
            cout << "Enter Time: ";
            cin >> hour;
            cout << "Enter Room: ";
            cin >> room;

            if (Delete(root, makeKey(date, hour, room))){
                RewriteFile(root);
                cout << "Booking cancelled.\n";
            }
            else
                cout << "Booking not found.\n";
        }

        // Search booking
        else if (choice == 3) {
            string date, room;
            int hour;
            Booking b;

            cout << "Enter Date (YYMMDD): ";
            cin >> date;
            cout << "Enter Time: ";
            cin >> hour;
            cout << "Enter Room: ";
            cin >> room;

            if (Search(root, makeKey(date, hour, room), b)) {
                cout << "\n--- Booking Found ---\n";
                cout << "Lecturer: " << b.lecturer << endl;
                cout << "Course: " << b.course << endl;
            } else {
                cout << "Booking not found.\n";
            }
        }

        // Display all bookings
        else if (choice == 4) {
            cout << "\n===========================================================\n";
            cout << "|  Date  | Time | Room   | Lecturer     | Course     |\n";
            cout << "===========================================================\n";
            Display(root);
            cout << "===========================================================\n";
        }
        
        else if (choice == 5) {
            string date;
            cout << "Enter Date (YYMMDD): ";
            cin >> date;

            cout << "\n===========================================================\n";
            cout << "|  Date  | Time | Room   | Lecturer     | Course     |\n";
            cout << "===========================================================\n";

            DisplayByDate(root, date);

            cout << "===========================================================\n";
        }

        else if (choice == 6) {
            string room;
            cout << "Enter Room: ";
            cin >> room;

            cout << "\n===========================================================\n";
            cout << "|  Date  | Time | Room   | Lecturer     | Course     |\n";
            cout << "===========================================================\n";

            DisplayByRoom(root, room);

            cout << "===========================================================\n";
        }

    } while (choice != 7);

    return 0;
}
