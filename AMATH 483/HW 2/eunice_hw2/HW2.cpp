#include <iostream>
#include <iomanip>
#include <cmath>     
#include <limits>

int main() {
    // Problem 1
    float epsSP = 0.0f;       
    int j = 1;                
    float diffSP = 0.0f;
    while (true) {float term = 1.0f / powf(2.0f, j); // Compute 1/2^j using powf 
        // Compute the difference: (1 + 1/2^j) - 1
        diffSP = (1.0f + term) - 1.0f;
        // If the difference is 0, then term is too small to be distinguished
        if (diffSP == 0.0f) {epsSP = 1.0f / powf(2.0f, j - 1);
            break;}
        // Otherwise, increment j and repeat.
        j++;}
    
    double epsDP = 0.0;      
    j = 1;                  // Reset counter for double computation
    double diffDP = 0.0;
    while (true) {
        // Compute 1/2^j
        double term = 1.0 / pow(2.0, j);
        // Compute the difference: (1 + 1/2^j) - 1
        diffDP = (1.0 + term) - 1.0;
        // When the term is too small, diffDP will be 0.
        if (diffDP == 0.0) {epsDP = 1.0 / pow(2.0, j - 1);
            break;}
        j++;}

    std::cout << std::scientific << std::setprecision(6);
    std::cout << "Problem 1 Solution: "<< std::endl;
    std::cout << "Practical measure of your machine's SP (32 bit) = " 
              << std::numeric_limits<float>::epsilon() << std::endl;
    std::cout << "Practical measure of your machine's DP (64 bit) = " 
              << std::numeric_limits<double>::epsilon() << std::endl;



    // Problem 2
    float k_SP = 8;
    float n_SP = 23;
    // Largest SP number
    float f_largeSP = 1 - std::pow(2.0f, -n_SP);
    float M_largeSP = 2 - std::pow(2.0f, -n_SP);
    float E_largeSP = std::pow(2.0f, k_SP-1) - 1;
    float V_largeSP = M_largeSP * std::pow(2.0f, E_largeSP);
    std::cout << "Problem 2 Solution: "<< std::endl;
    std::cout << "1) Largest SP number in IEEE floating point representation = " << V_largeSP << std::endl;


    // Smallest SP number
    float M_smallSP = 1;
    float E_smallSP = -std::pow(2.0f, k_SP-1) + 2;
    float V_smallSP = M_smallSP * std::pow(2.0f, E_smallSP);
    std::cout << "2) Smallest SP number in IEEE floating point representation = " << V_smallSP << std::endl;


    // Largest DP number
    double k_DP = 11;
    double n_DP = 52;

    double f_largeDP = 1 - std::pow(2.0, -n_DP);
    double M_largeDP = 2 - std::pow(2.0, -n_DP);
    double E_largeDP = std::pow(2.0, k_DP-1) - 1;
    double V_largeDP = M_largeDP * std::pow(2.0, E_largeDP);
    std::cout << "3) Largest DP number in IEEE floating point representation = " << V_largeDP << std::endl;




    // Smallest DP number
    double M_smallDP = 1;
    double E_smallDP = -std::pow(2.0, k_DP-1) + 2;
    double V_smallDP = M_smallDP * std::pow(2.0, E_smallDP);
    std::cout << "4) Smallest DP number in IEEE floating point representation = " << V_smallDP << std::endl;



    // Problem 3
    int a = 200, b = 300, c = 400, d = 500;
    int product_pbm3 = a * b * c * d;
    std::cout << "Problem 3 Solution: " << std::endl;
    std::cout << "Multiply the integers, 200 * 300 * 400 * 500 = " << product_pbm3 << std::endl;

    // The result of multiplying the integers, 200 * 300 * 400 * 500 is -884901888. This result is different to the mathmetical multiplication which is 12,000,000,000.
    // The reason is because 12,000,000,000 is greater than the maximum that 32-bit signed interger range which is [2^(-31), 2^31-1]. 
    // In these cases, C++ wraps around modulo 2^32 which is called as integer overflow.
    // So C++ computes the multiplication by 12,000,000,000 mod 2^32. 
    // 2^32 = 4,294,967,296 and as 4,294,967,296 goes twice into 12,000,000,000. Finding the remainder of this, 12,000,000,000 - (4,294,967,296 * 2) = 3,410,065,408.
    // Any 32-bit value above 2^31-1 has the sign bit set to 1, which would appear negative. 
    // So to get the signed value, it would be signedvalue = unsignedvalue - 2^32 = 3,410,065,408−4,294,967,296=−884,901,888.
    // Therefore, C++ gets the unsigned results of 12,000,000,000 mod 2^32 = 3,410,065,408, but as "int" is signed, 
    // the computer sees the patterns as 32-bit pattern which is 3,410,065,408−4,294,967,296=−884,901,888.
    // The effect I observe is the " Integer Overflow."


    // Problem 4
    unsigned int counter = 0;
    for (int i = 0; i < 3; ++i) 
        -- counter;

    std::cout << "Problem 4 Solution: " << std::endl;
    std::cout << "counter = " << counter << std::endl;




    // Problem 5
    


    // Problem 6




    // Problem 7



    // Problem 8



    // Problem 9


    return 0;
}



