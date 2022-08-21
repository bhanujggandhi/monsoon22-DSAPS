#include <iostream>
#include <string>

using namespace std;

// Utilities

// Utility to remove leading zeroes
string normalize(string s)
{
    int k = 0;
    for (int i = 0; i < s.size(); i++)
    {
        int a = s[i] - '0';
        if (a > 0 and a <= 9)
        {
            k = i;
            break;
        }
    }

    string ans = s.substr(k);

    return ans;
}

// Check if num1 > num2
bool isGreater(string num1, string num2)
{
    num1 = normalize(num1);
    num2 = normalize(num2);
    int size1 = num1.size();
    int size2 = num2.size();

    if (size1 < size2)
        return false;
    else if (size1 > size2)
        return true;
    else
    {
        int i = 0;

        while (i < size1)
        {
            int a = num1[i] - '0';
            int b = num2[i] - '0';
            if (a > b)
                return true;
            else if (b > a)
                return false;
            else
                i++;
        }
        return false;
    }
}

// Check is num == 0
bool isZero(string num)
{
    num = normalize(num);
    int n = num.size();

    if (n == 0)
        return true;

    for (int i = 0; i < n; i++)
    {
        if (num[i] != '0')
            return false;
    }
    return true;
}

// Check if num1 == num2
bool isEqual(string num1, string num2)
{
    num1 = normalize(num1);
    num2 = normalize(num2);

    int size1 = num1.size();
    int size2 = num2.size();

    if (size1 < size2)
        return false;
    else if (size1 > size2)
        return false;
    else
    {
        int i = 0;

        while (i < size1)
        {
            int a = num1[i] - '0';
            int b = num2[i] - '0';
            if (a > b)
                return false;
            else if (b > a)
                return false;
            else
                i++;
        }
        return true;
    }
}

// Reverse string
void reverse_string(string &s)
{
    int n = s.size();
    for (int i = 0; i < n / 2; i++)
    {
        char temp = s[i];
        s[i] = s[n - i - 1];
        s[n - i - 1] = temp;
    }
    return;
}

// Returns num1+num2
string bigintadd(string num1, string num2)
{
    reverse_string(num1);
    reverse_string(num2);
    string ans;

    int size1 = num1.size();
    int size2 = num2.size();
    int i = 0, j = 0, carry = 0;

    while (i < size1 and j < size2)
    {
        int a = num1[i++] - '0';
        int b = num2[j++] - '0';
        int c = (a + b + carry) % 10;
        carry = (a + b + carry) / 10;
        ans.push_back(c + '0');
    }

    while (i < size1)
    {
        int a = num1[i++] - '0';
        int c = (a + carry) % 10;
        carry = (a + carry) / 10;
        ans.push_back(c + '0');
    }

    while (j < size2)
    {
        int a = num2[j++] - '0';
        int c = (a + carry) % 10;
        carry = (a + carry) / 10;
        ans.push_back(c + '0');
    }

    if (carry != 0)
        ans.push_back(carry + '0');

    reverse_string(ans);
    return ans;
}

// Returns num1-num2 (assumption num1 >= num2 always)
string bigintsubtract(string num1, string num2)
{
    reverse_string(num1);
    reverse_string(num2);
    string ans;

    int size1 = num1.size();
    int size2 = num2.size();
    int i = 0, j = 0, borrow = 0;

    while (i < size1 and j < size2)
    {
        int a = num1[i++] - '0';
        int b = num2[j++] - '0';
        int c = a - b + borrow;
        if (c < 0)
        {
            borrow = -1;
            c = c + 10;
        }
        else
        {
            borrow = 0;
        }
        ans.push_back(c + '0');
    }

    while (i < size1)
    {
        int a = num1[i++] - '0';
        int c = a + borrow;
        if (c < 0)
        {
            borrow = -1;
            c = c + 10;
        }
        else
        {
            borrow = 0;
        }
        ans.push_back(c + '0');
    }

    reverse_string(ans);
    return normalize(ans);
}

// Returns num1 x num2
string bigintmultiply(string num1, string num2)
{
    reverse_string(num1);
    reverse_string(num2);
    string ans = "0";

    long long size1 = num1.size();
    long long size2 = num2.size();
    int carry = 0;

    for (long long j = 0; j < size2; j++)
    {
        string temp;
        for (long long i = 0; i < size1; i++)
        {
            int a = num1[i] - '0';
            int b = num2[j] - '0';
            int c = ((a * b) + carry) % 10;
            carry = ((a * b) + carry) / 10;

            temp.push_back(c + '0');
        }
        if (carry != 0)
            temp.push_back(carry + '0');
        carry = 0;
        reverse_string(temp);
        for (long long i = 0; i < j; i++)
        {
            temp.push_back('0');
        }
        ans = bigintadd(temp, ans);
    }

    return ans;
}

// Returns num1 % num2
string remainder(string num1, string num2)
{
    if (isEqual(num1, num2))
        return "0";

    if (isGreater(num2, num1))
        return num1;

    string remain = "";
    string quotient = "";
    int sz = num2.size();

    int ind = 0;
    while (ind < num1.size())
    {
        while (remain.size() != sz and ind < num1.size())
            remain.push_back(num1[ind++]);

        if (isGreater(num2, remain))
        {
            if (ind >= num1.size())
                return normalize(remain);
            remain += num1[ind++];
            quotient += '0';
        }
        int q = 0;
        while (isGreater(remain, num2))
        {
            remain = normalize(bigintsubtract(remain, num2));
            q++;
        }
        quotient += (q + '0');
    }

    return remain;
}

/*
=================================================================================
*/

// 1. Expression Evaluation
string solveexpression(string &s)
{
    int n = s.size();
    int count = 0;

    for (int i = 0; i < n; i++)
        if (s[i] == '+' or s[i] == '-' or s[i] == 'x')
            count++;

    count = (2 * count) + 1;

    string *expressionarr = new string[count];

    int start = 0;
    int end = 0;
    int idx = 0;
    for (int i = 0; i < n; i++)
    {
        if (s[i] != '+' and s[i] != '-' and s[i] != 'x')
            continue;

        expressionarr[idx++] = s.substr(start, i - start);
        expressionarr[idx++] = s[i];
        start = i + 1;
    }

    expressionarr[idx] = s.substr(start);

    // solve all the multiplications
    for (int i = 0; i < count; i++)
    {
        string temp;
        if (expressionarr[i] == "x")
        {
            temp = bigintmultiply(expressionarr[i - 1], expressionarr[i + 1]);
            expressionarr[i - 1] = "$";
            expressionarr[i] = "$";
            expressionarr[i + 1] = temp;
        }
    }

    // Count how many numbers and operators are left
    int countx = 0;
    for (int i = 0; i < count; i++)
    {
        if (expressionarr[i] != "$")
            countx++;
    }

    // Fill all the numbers and operators
    string *exprarr = new string[countx];

    idx = 0;
    for (int i = 0; i < count; i++)
        if (expressionarr[i] != "$")
            exprarr[idx++] = expressionarr[i];

    delete[] expressionarr;

    // Solve + and -
    for (int i = 0; i < countx; i++)
    {
        string temp;
        if (exprarr[i] == "+")
        {
            temp = bigintadd(exprarr[i - 1], exprarr[i + 1]);
            exprarr[i - 1] = "$";
            exprarr[i] = "$";
            exprarr[i + 1] = temp;
        }

        if (exprarr[i] == "-")
        {
            temp = bigintsubtract(exprarr[i - 1], exprarr[i + 1]);
            exprarr[i - 1] = "$";
            exprarr[i] = "$";
            exprarr[i + 1] = temp;
        }
    }

    string ans = exprarr[countx - 1];
    delete[] exprarr;
    return ans;
}

// 2. Exponent
string bigintexponent(string &base, long long power)
{
    if (power == 0)
        return "1";

    if (power % 2 == 1)
    {
        string y = bigintexponent(base, power - 1);
        return bigintmultiply(y, base);
    }
    else
    {
        string y = bigintexponent(base, power / 2);
        return bigintmultiply(y, y);
    }
}

// 3. GCD of BigInt
string bigintgcd(string num1, string num2)
{
    if (isZero(num1))
        return num2;
    if (isZero(num2))
        return num1;

    if (isEqual(num1, num2))
        return num1;

    return bigintgcd(remainder(num2, num1), num1);
}

// 4. Factorial
void bigintfactorial(string &s)
{
    string ans = "1";
    long long n = stoi(s);
    for (int i = 2; i <= n; i++)
    {
        ans = bigintmultiply(ans, to_string(i));
    }

    cout << ans << endl;
}

int main()
{
    int operation;
    string num1;
    string num2;
    long long power;

    cout << "--------------------------------------------------" << endl;
    cout << "|  Choose an operation to perform                |" << endl;
    cout << "|  1. Solve a infix expression                   |" << endl;
    cout << "|  2. Evaluate exponent                          |" << endl;
    cout << "|  3. Evaluate GCD of two Big Integers           |" << endl;
    cout << "|  4. Evaluate factorial of a Big Integer        |" << endl;
    cout << "--------------------------------------------------" << endl;

    cout << "Enter an operation: ";
    cin >> operation;
    if (operation == 1)
    {
        cout << "Enter the expression" << endl;
        cin >> num1;
        cout << solveexpression(num1) << endl;
    }
    else if (operation == 2)
    {
        cout << "Enter a Big Integer" << endl;
        cin >> num1;
        cout << "Enter the exponent (<2^63)" << endl;
        cin >> power;
        cout << bigintexponent(num1, power) << endl;
    }
    else if (operation == 3)
    {
        cout << "Enter a Big Integer" << endl;
        cin >> num1;
        cout << "Enter another Big Integer" << endl;
        cin >> num2;
        cout << bigintgcd(num1, num2) << endl;
    }
    else if (operation == 4)
    {
        cout << "Enter a Big Integer" << endl;
        cin >> num1;
        bigintfactorial(num1);
    }
    else
    {
        cout << "Invalid Input" << endl;
        exit(0);
    }

    return 0;
}