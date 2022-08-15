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

    cout << size1 << endl;
    cout << size2 << endl;

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

    cout << num1 << "   " << num2 << endl;

    int size1 = num1.size();
    int size2 = num2.size();
    int carry = 0;

    for (int j = 0; j < size2; j++)
    {
        string temp;
        for (int i = 0; i < size1; i++)
        {
            int a = num1[i] - '0';
            int b = num2[j] - '0';
            int c = ((a * b) + carry) % 10;
            carry = ((a * b) + carry) / 10;
            temp.push_back(c + '0');
        }
        reverse_string(temp);
        for (int i = 0; i < j; i++)
        {
            temp.push_back('0');
        }
        cout << temp << "   " << ans << endl;
        ans = bigintadd(temp, ans);
    }

    return ans;
}

int main()
{
    int n;
    cin >> n;
    string inp;
    cin >> inp;

    // switch (n)
    // {
    // case 1:
    //     cout << "Called 1";
    //     break;
    // case 2:
    //     cout << "Called 2";
    //     break;
    // case 3:
    //     cout << "Called 3";
    //     break;
    // case 4:
    //     cout << "Called 4";
    //     break;
    // default:
    //     cout << "Called default";
    //     break;
    // }

    string ans = bigintmultiply("12345678", "56781234");
    // string ans = bigintadd("99", "1");
    cout << ans << endl;

    return 0;
}