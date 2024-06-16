#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <limits>
#include <conio.h>
#include <string>
#include <algorithm>

using namespace std;

// Constants
const double kPi = 3.14159265358979323846;
const double kTolerance = 0.01;
const streampos kHFOVOffset = 0x001AF751;
const streampos kVFOVOffset = 0x001AF761;
const float kDefaultVFOVInRadians = 1.1780972480773926;

// Variables
int choice1, choice2, fileOpened, tempChoice;
bool fileNotFound, validKeyPressed;
double oldWidth = 4.0, oldHeight = 3.0, oldHFOV = 90.0, oldAspectRatio = oldWidth / oldHeight, newAspectRatio, newWidth, newHeight, currentHFOVInDegrees, currentVFOVInDegrees, newHFOVInDegrees, newVFOVInDegrees, newCustomFOVInDegrees, newCustomResolutionValue;
float currentHFOVInRadians, currentVFOVInRadians, newHFOVInRadians, newVFOVInRadians;
string descriptor, fovDescriptor, input;
fstream file;
char ch;

// Function to convert degrees to radians
double DegToRad(double degrees)
{
    return degrees * (kPi / 180.0);
}

// Function to convert radians to degrees
double RadToDeg(double radians)
{
    return radians * (180.0 / kPi);
}

// Function to handle user input in choices
void HandleChoiceInput(int &choice)
{
    tempChoice = -1;         // Temporary variable to store the input
    validKeyPressed = false; // Flag to track if a valid key was pressed

    while (true)
    {
        ch = _getch(); // Waits for user to press a key

        // Checks if the key is '1' or '2'
        if ((ch == '1' || ch == '2') && !validKeyPressed)
        {
            tempChoice = ch - '0';  // Converts char to int and stores the input temporarily
            cout << ch;             // Echoes the valid input
            validKeyPressed = true; // Sets the flag as a valid key has been pressed
        }
        else if (ch == '\b' || ch == 127) // Handles backspace or delete keys
        {
            if (tempChoice != -1) // Checks if there is something to delete
            {
                tempChoice = -1;         // Resets the temporary choice
                cout << "\b \b";         // Erases the last character from the console
                validKeyPressed = false; // Resets the flag as the input has been deleted
            }
        }
        // If 'Enter' is pressed and a valid key has been pressed prior
        else if (ch == '\r' && validKeyPressed) 
        {
            choice = tempChoice; // Assigns the temporary input to the choice variable
            cout << endl;        // Moves to a new line
            break;               // Exits the loop since we have a confirmed input
        }
    }
}

float HandleFOVInput()
{
    do
    {
        // Reads the input as a string
        cin >> input;

        // Replaces all commas with dots
        replace(input.begin(), input.end(), ',', '.');

        // Parses the string to a float
        newCustomFOVInDegrees = stof(input);

        if (cin.fail())
        {
            cin.clear();                                         // Clears error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores invalid input
            cout << "Invalid input. Please enter a numeric value." << endl;
        }
        else if (newCustomFOVInDegrees <= 0 || newCustomFOVInDegrees >= 180)
        {
            cout << "Please enter a valid number for the FOV (greater than 0 and less than 180)." << endl;
        }
    } while (newCustomFOVInDegrees <= 0 || newCustomFOVInDegrees >= 180);

    return newCustomFOVInDegrees;
}

double HandleResolutionInput()
{
    do
    {
        cin >> newCustomResolutionValue;

        cin.clear();                                         // Clears error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores invalid input

        if (cin.fail())
        {
            cin.clear();                                         // Clears error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores invalid input
            cout << "Invalid input. Please enter a numeric value." << endl;
        }
        else if (newCustomResolutionValue <= 0 || newCustomResolutionValue >= 65535)
        {
            cout << "Please enter a valid number." << endl;
        }
    } while (newCustomResolutionValue <= 0 || newCustomResolutionValue > 65535);

    return newCustomResolutionValue;
}

// Function to open the file
void OpenFile(fstream &file)
{
    fileNotFound = false;
    fileOpened = 0; // Initializes fileOpened to 0

    file.open("CShell.dll", ios::in | ios::out | ios::binary);

    // If the file is not open, sets fileNotFound to true
    if (!file.is_open())
    {
        fileNotFound = true;
    }

    // Loops until the file is found and opened
    while (fileNotFound)
    {

        // Tries to open the file again
        file.open("CShell.dll", ios::in | ios::out | ios::binary);

        if (!file.is_open())
        {
            cout << "\nFailed to open CShell.dll, check if the DLL file has special permissions allowed that prevent the fixer from opening it (e.g: read-only mode), it's not present in the same directory as the fixer, or if the DLL is currently being used. Press Enter when all the mentioned problems are solved." << endl;
            do
            {
                ch = _getch(); // Wait for user to press a key
            } while (ch != '\r'); // Keep waiting if the key is not Enter ('\r' is the Enter key in ASCII)
        }
        else
        {
            cout << "\nCShell.dll opened successfully!" << endl;
            fileNotFound = false; // Sets fileNotFound to false as the file is found and opened
        }
    }
}

int main()
{
    cout << "Arthur's Quest: Battle for the Kingdom (2002) FOV Fixer v1.2 by AlphaYellow, 2024\n\n----------------\n";

    do
    {
        OpenFile(file);

        file.seekg(kHFOVOffset);
        file.read(reinterpret_cast<char *>(&currentHFOVInRadians), sizeof(currentHFOVInRadians));
        file.seekg(kVFOVOffset);
        file.read(reinterpret_cast<char *>(&currentVFOVInRadians), sizeof(currentVFOVInRadians));

        file.close();

        // Converts the FOV values from radians to degrees
        currentHFOVInDegrees = RadToDeg(currentHFOVInRadians);
        currentVFOVInDegrees = RadToDeg(currentVFOVInRadians);

        // Checks if the FOV values correspond to exactly 90 (horizontal), and exactly or approximately 67.5 (vertical) degrees
        fovDescriptor = "";
        if (abs(currentHFOVInDegrees - 90.0) < numeric_limits<float>::epsilon() &&
            abs(currentVFOVInDegrees - 67.5) < kTolerance)
        {
            fovDescriptor = "[Default for 4:3 aspect ratio]";
        }

        cout << "\nYour current FOV: " << currentHFOVInDegrees << " degrees (Horizontal); " << currentVFOVInDegrees << " degrees (Vertical) " << fovDescriptor << "\n";

        cout << "\n- Do you want to set FOV automatically based on the desired resolution (1) or set custom horizontal and vertical FOV values (2)?: ";
        HandleChoiceInput(choice1);

        switch (choice1)
        {
        case 1:
            cout << "\n- Enter the desired width: ";
            newWidth = HandleResolutionInput();

            cout << "\n- Enter the desired height: ";
            newHeight = HandleResolutionInput();

            newAspectRatio = newWidth / newHeight;

            // Calculates the new horizontal FOV
            newHFOVInDegrees = 2.0 * RadToDeg(atan((newAspectRatio / oldAspectRatio) * tan(DegToRad(oldHFOV / 2.0))));

            newHFOVInRadians = static_cast<float>(DegToRad(newHFOVInDegrees)); // Converts degrees to radians

            newVFOVInRadians = kDefaultVFOVInRadians;

            newVFOVInDegrees = RadToDeg(newVFOVInRadians);

            descriptor = "automatically";

            break;

        case 2:
            cout << "\n- Enter the desired horizontal FOV (in degrees): ";
            newHFOVInDegrees = HandleFOVInput();

            cout << "\n- Enter the desired vertical FOV (in degrees): ";
            newVFOVInDegrees = HandleFOVInput();

            newHFOVInRadians = static_cast<float>(DegToRad(newHFOVInDegrees)); // Converts degrees to radians

            newVFOVInRadians = static_cast<float>(DegToRad(newVFOVInDegrees)); // Converts degrees to radians

            descriptor = "manually";

            break;
        }

        OpenFile(file);

        file.seekp(kHFOVOffset);
        file.write(reinterpret_cast<const char *>(&newHFOVInRadians), sizeof(newHFOVInRadians));
        file.seekp(kVFOVOffset);
        file.write(reinterpret_cast<const char *>(&newVFOVInRadians), sizeof(newVFOVInRadians));

        // Confirmation message
        cout << "\nSuccessfully changed " << descriptor << " the horizontal FOV to " << newHFOVInDegrees << " degrees and vertical FOV to " << newVFOVInDegrees << " degrees."
             << endl;

        // Closes the file
        file.close();

        cout << "\n- Do you want to exit the program (1) or try another value (2)?: ";
        HandleChoiceInput(choice2);

        if (choice2 == 1)
        {
            cout << "\nPress enter to exit the program...";
            do
            {
                ch = _getch(); // Wait for user to press a key
            } while (ch != '\r'); // Keep waiting if the key is not Enter ('\r' is the Enter key in ASCII)
            return 0;
        }
    } while (choice2 == 2); // Checks the flag in the loop condition
}