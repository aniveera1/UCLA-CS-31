//
//  CS 31 Project 2
//  License to Bill
//
//  Created by Anirudh Veeraragavan on 10/6/16.
//
// The purpose of this program is to calculate the licensing fee of properties given the property's identification, expected revenue, and country location.

#include <iostream>
#include <string>
using namespace std;

int main()
{
    // Variable Declarations
    string property_identification;
    double property_revenue;
    string property_location;
    
    const double LOWER_TIER_FEE = .181;
    const double MIDDLE_TIER_FEE = .203;
    const double SPECIAL_MIDDLE_TIER_FEE = .217;
    const double UPPER_TIER_FEE = .23;

    double license_fee;
    
    // User Inputs
    cout << "Identification: ";
    getline (cin, property_identification);
    if (property_identification == "") // Error message
    {
        cout << "---" << endl;
        cout << "You must enter a property identification." << endl;
        return 1;
    }
    
    cout << "Expected revenue (in millions): ";
    cin >> property_revenue;
    if (property_revenue < 0) // Error message
    {
        cout << "---" << endl;
        cout << "The expected revenue must be nonnegative." << endl;
        return 1;
    }
    
    cout << "Country: ";
    cin.ignore(10000, '\n'); // Needed because the previous user input was an integer
    getline (cin, property_location);
    if (property_location == "") // Error message
    {
        cout << "---" << endl;
        cout << "You must enter a country." << endl;
        return 1;
    }
    
    cout << "---" << endl;
    
    // Licensing Fee Calculation
    bool location = (property_location == "UAE" || property_location == "Turkey"); // Needed to determine if the location is in UAE or Turkey
    
    if (property_revenue > 50 and location)
    {
        double upper_tier_revenue = (property_revenue - 50) * UPPER_TIER_FEE;
        double middle_tier_revenue = 30 * SPECIAL_MIDDLE_TIER_FEE;
        double lower_tier_revenue = 20 * LOWER_TIER_FEE;
        license_fee = upper_tier_revenue + middle_tier_revenue + lower_tier_revenue;
    }
    else if (property_revenue > 50)
    {
        double upper_tier_revenue = (property_revenue - 50) * UPPER_TIER_FEE;
        double middle_tier_revenue = 30 * MIDDLE_TIER_FEE;
        double lower_tier_revenue = 20 * LOWER_TIER_FEE;
        license_fee = upper_tier_revenue + middle_tier_revenue + lower_tier_revenue;
    }
    else if (property_revenue > 20 and location)
    {
        double middle_tier_revenue = (property_revenue - 20) * SPECIAL_MIDDLE_TIER_FEE;
        double lower_tier_revenue = 20 * LOWER_TIER_FEE;
        license_fee = middle_tier_revenue + lower_tier_revenue;
    }
    else if (property_revenue > 20)
    {
        double middle_tier_revenue = (property_revenue - 20) * MIDDLE_TIER_FEE;
        double lower_tier_revenue = 20 * LOWER_TIER_FEE;
        license_fee = middle_tier_revenue + lower_tier_revenue;
    }
    else
    {
        double lower_tier_revenue = (property_revenue * LOWER_TIER_FEE);
        license_fee = lower_tier_revenue;
    }
    
    cout.setf(ios::fixed);
    cout.precision(3); // Needed to ensure that license_fee has three places to the right of the decimal point
    
    cout << "The license fee for " << property_identification << " is $" << license_fee << " million." << endl;
}
