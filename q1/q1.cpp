#include <iostream>
#include <string>

using namespace std;

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
    return ans;
}

string bigintmultiply(string num1, string num2)
{
    reverse_string(num1);
    reverse_string(num2);
    string ans = "0";
    // cout << num1 << "   " << num2 << endl;

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

            // cout << c << "--->" << carry << endl;
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
        // cout << temp << "        " << ans << endl;
        ans = bigintadd(temp, ans);
    }

    return ans;
}

void solveexpression(string &s)
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
        // cout << expressionarr[i] << endl;
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

    cout << exprarr[countx - 1] << endl;
}

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
    int n;
    cin >> n;
    string inp;
    cin >> inp;

    switch (n)
    {
    case 1:
        solveexpression(inp);
        break;
    case 2:
        cout << "Called 2";
        break;
    case 3:
        cout << "Called 3";
        break;
    case 4:
        bigintfactorial(inp);
        break;
    default:
        cout << "Called default";
        break;
    }

    // cout << bigintmultiply("99893271223", "9203232392") << endl;

    return 0;
}