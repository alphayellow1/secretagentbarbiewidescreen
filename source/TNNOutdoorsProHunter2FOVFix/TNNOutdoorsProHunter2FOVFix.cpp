#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <limits>

// Define M_PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Function to convert degrees to radians
double degToRad(double degrees)
{
    return degrees * (M_PI / 180.0);
}

// Function to convert radians to degrees
double radToDeg(double radians)
{
    return radians * (180.0 / M_PI);
}

using namespace std;

int main()
{
    double oldWidth = 4.0;
    double oldHeight = 3.0;
    double oldAspectRatio = oldWidth / oldHeight;
    double oldHorizontalFOV = 90.0; // Known horizontal FOV for 4:3 aspect ratio
    double newHorizontalFOV;
    double newAspectRatio;

    int choice;
    double newWidth, newHeight, result, horizontalFOV, verticalFOV;
    float horizontalFovInRadians1, verticalFovInRadians1, horizontalFovInRadians2, verticalFovInRadians2;
    float currentHFOV, currentVFOV, newHFOV1, newVFOV1, newHFOV2, newVFOV2;

    cout << "TNN Outdoors Pro Hunter 2 (1999) FOV Fixer by AlphaYellow, 2024\n\n----------------\n\n";

    fstream file("CLIENT.EXE", ios::in | ios::out | ios::binary);
    if (!file.is_open())
    {
        cout << "Failed to open the file." << endl;
        return 1;
    }

    file.seekg(0x0007B646);
    file.read(reinterpret_cast<char *>(&currentHFOV), sizeof(currentHFOV));
    file.seekg(0x0007B666);
    file.read(reinterpret_cast<char *>(&currentVFOV), sizeof(currentVFOV));

    // Defines a tolerance for the approximation
    const double tolerance = 0.01;

    // Converts the FOV values from radians to degrees
    currentHFOV = radToDeg(currentHFOV);
    currentVFOV = radToDeg(currentVFOV);

    // Checks if the FOV values correspond to exactly 90 (horizontal), and exactly or approximately 75 (vertical) degrees
    string fovDescriptor = "";
    if (abs(currentHFOV - 90.0) < numeric_limits<float>::epsilon() &&
        abs(currentVFOV - 75.0) < tolerance)
    {
        fovDescriptor = "[Default for 4:3 aspect ratio]";
    }

    cout << "- Your current FOV: " << currentHFOV << " degrees (Horizontal); " << currentVFOV << " degrees (Vertical) " << fovDescriptor << "\n\n";
    file.close();

    do
    {
        cout << "- Do you want to set FOV automatically based on the desired resolution (1) or set custom horizontal and vertical FOV values? (2): ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();                                         // Clears error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores invalid input
            choice = -1;                                         // Ensures the loop continues
            cout << "Invalid input. Please enter a numeric value." << endl;
        }
        else if (choice < 1 || choice > 2)
        {
            cout << "Please enter a valid number." << endl;
        }
    } while (choice < 1 || choice > 2);

    if (choice == 1)
    {
        do
        {
            cout << "\n- Enter the desired width: ";
            cin >> newWidth;

            if (cin.fail())
            {
                cin.clear();                                         // Clears error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores invalid input
                newWidth = -1;                                       // Ensures the loop continues
                cout << "Invalid input. Please enter a numeric value." << endl;
            }
            else if (newWidth <= 0 || newWidth > 65535)
            {
                cout << "Please enter a positive number for width less than 65536." << endl;
            }
        } while (newWidth <= 0 || newWidth > 65535);

        do
        {
            cout << "\n- Enter the desired height: ";
            cin >> newHeight;

            if (cin.fail())
            {
                cin.clear();                                         // Clears error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores invalid input
                newHeight = -1;                                      // Ensures the loop continues
                cout << "Invalid input. Please enter a numeric value." << endl;
            }
            else if (newHeight <= 0 || newHeight > 65535)
            {
                cout << "Please enter a positive number for height less than 65536." << endl;
            }
        } while (newHeight <= 0 || newHeight > 65535);

        newAspectRatio = newWidth / newHeight;

        // Calculates the new horizontal FOV
        newHorizontalFOV = 2.0 * radToDeg(atan((newAspectRatio / oldAspectRatio) * tan(degToRad(oldHorizontalFOV / 2.0))));

        fstream file("CLIENT.EXE", ios::in | ios::out | ios::binary);
        if (!file.is_open())
        {
            cout << "Failed to open the file." << endl;
            return 1;
        }

        horizontalFovInRadians1 = static_cast<float>(newHorizontalFOV * (M_PI / 180.0)); // Converts degrees to radians

        verticalFovInRadians1 = 1.3089969158172607;

        file.seekp(0x0007B646);
        file.write(reinterpret_cast<const char *>(&horizontalFovInRadians1), sizeof(horizontalFovInRadians1));
        file.seekp(0x0007B666);
        file.write(reinterpret_cast<const char *>(&verticalFovInRadians1), sizeof(verticalFovInRadians1));

        newHFOV1 = radToDeg(horizontalFovInRadians1);
        newVFOV1 = radToDeg(verticalFovInRadians1);

        // Confirmation message
        cout << "\nSuccessfully changed automatically the horizontal FOV to " << newHFOV1 << " degrees and vertical FOV to " << newVFOV1 << " degrees.\n"
             << endl;

        // Closes the file
        file.close();
    }
    else
    {
        do
        {
            cout << "\n- Enter the desired horizontal FOV (in degrees): ";
            cin >> horizontalFOV;

            if (cin.fail())
            {
                cin.clear();                                         // Clears error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores invalid input
                horizontalFOV = -1;                                  // Ensures the loop continues
                cout << "Invalid input. Please enter a numeric value." << endl;
            }
            else if (horizontalFOV <= 0 || horizontalFOV >= 180)
            {
                cout << "Please enter a valid number for the horizontal FOV." << endl;
            }
        } while (horizontalFOV <= 0 || horizontalFOV >= 180);

        do
        {
            cout << "\n- Enter the desired vertical FOV (in degrees): ";
            cin >> verticalFOV;

            if (cin.fail())
            {
                cin.clear();                                         // Clears error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores invalid input
                verticalFOV = -1;                                    // Ensures the loop continues
                cout << "Invalid input. Please enter a numeric value." << endl;
            }
            else if (verticalFOV <= 0 || verticalFOV >= 180)
            {
                cout << "Please enter a valid number for the vertical FOV." << endl;
            }
        } while (verticalFOV <= 0 || verticalFOV >= 180);

        fstream file("CLIENT.EXE", ios::in | ios::out | ios::binary);
        if (!file.is_open())
        {
            cout << "Failed to open the file." << endl;
            return 1;
        }

        horizontalFovInRadians2 = static_cast<float>(horizontalFOV * (M_PI / 180.0)); // Converts degrees to radians
        verticalFovInRadians2 = static_cast<float>(verticalFOV * (M_PI / 180.0));     // Converts degrees to radians

        file.seekp(0x0007B646);
        file.write(reinterpret_cast<const char *>(&horizontalFovInRadians2), sizeof(horizontalFovInRadians2));
        file.seekp(0x0007B666);
        file.write(reinterpret_cast<const char *>(&verticalFovInRadians2), sizeof(verticalFovInRadians2));

        newHFOV2 = radToDeg(horizontalFovInRadians2);
        newVFOV2 = radToDeg(verticalFovInRadians2);

        // Confirmation message
        cout << "\nSuccessfully changed manually the horizontal FOV to " << newHFOV2 << " degrees and vertical FOV to " << newVFOV2 << " degrees.\n"
             << endl;

        // Closes the file
        file.close();
    }

    cout << "Press enter to exit the program...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clears the input buffer
    cin.get();

    return 0;
}