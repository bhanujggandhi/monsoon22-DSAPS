#include <iostream>

using namespace std;

void reverse(string &str) {
    int l = 0;
    int r = str.size() - 1;

    while (l < r) {
        swap(str[r], str[l]);
        l++;
        r--;
    }
}

// remove all leading zeros
string removeZeroS(string a) {
    string res = "";
    int i = 0;
    while (i < a.size() - 1) {
        if (a[i] != '0') break;
        i++;
    }
    while (i < a.size()) {
        res.push_back(a[i]);
        i++;
    }

    return res;
}

string add(string a, string b) {
    a = removeZeroS(a);
    b = removeZeroS(b);
    string ans = "";
    int c = 0;
    int alen = a.length();
    int blen = b.length();
    if (alen < blen) {
        swap(a, b);
    }
    alen = a.length();
    blen = b.length();
    int temp;
    for (int i = blen - 1; i > -1; i--) {
        temp = (a[i + alen - blen] - '0') + (b[i] - '0') + c;
        ans.push_back(temp % 10 + '0');
        c = temp / 10;
    }

    for (int i = (alen - blen - 1); i > -1; i--) {
        temp = (a[i] - '0') + c;
        ans.push_back(temp % 10 + '0');
        c = temp / 10;
    }
    if (c != 0) {
        ans.push_back(c + '0');
    }
    reverse(ans);
    return ans;
}

bool isgreater(string a, string b) {
    a = removeZeroS(a);
    b = removeZeroS(b);
    if (a.length() > b.length())
        return true;
    else if (a.length() < b.length())
        return false;
    else {
        for (int i = 0; i < a.length(); i++) {
            int x = (a[i] - '0');
            int y = (b[i] - '0');
            if (x > y)
                return true;

            else if (x < y)
                return false;
        }
        return false;
    }
}

bool isequal(string a, string b) {
    a = removeZeroS(a);
    b = removeZeroS(b);
    if (a.length() != b.length())
        return false;
    else {
        for (int i = 0; i < a.length(); i++) {
            int x = (a[i] - '0');
            int y = (b[i] - '0');
            if (x != y) return false;
        }
        return true;
    }
}

string sub(string a, string b) {
    string ans = "";
    int bor = 0;
    int temp;
    int alen = a.length();
    int blen = b.length();
    if (alen < blen) {
        swap(a, b);
    }
    alen = a.length();
    blen = b.length();
    for (int i = blen - 1; i > -1; i--) {
        temp = (a[i + alen - blen] - '0') - bor;
        temp = temp - (b[i] - '0');
        if (temp < 0) {
            temp = 10 + temp;
            bor = 1;
        } else {
            bor = 0;
        }
        ans.push_back(temp % 10 + '0');
    }
    for (int i = (alen - blen - 1); i > -1; i--) {
        temp = (a[i] - '0') - bor;
        if (temp < 0) {
            temp = 10 + temp;
            bor = 1;
        } else {
            bor = 0;
        }
        ans.push_back(temp % 10 + '0');
    }
    reverse(ans);
    string res = "";
    int i = 0;
    while (i < ans.size() - 1) {
        if (ans[i] != '0') break;
        i++;
    }
    while (i < ans.size()) {
        res.push_back(ans[i]);
        i++;
    }

    return res;
}

string mul(string a, string b) {
    int l1 = a.length();
    int l2 = b.length();
    string ans = "";

    for (int i = l2 - 1; i > -1; i--) {
        int n1 = (b[i] - '0');
        int c = 0;
        string tempAns = "";
        int x = i;
        while (x < l2 - 1) {
            tempAns.push_back('0');
            x++;
        }
        for (int j = l1 - 1; j > -1; j--) {
            int temp2 = n1 * (a[j] - '0') + c;
            tempAns.push_back(temp2 % 10 + '0');
            c = temp2 / 10;
        }
        if (c != 0) {
            tempAns.push_back(c + '0');
        }

        reverse(tempAns);
        // cout << tempAns << endl;
        ans = add(ans, tempAns);
    }
    return ans;
}

string fact(string a) {
    // if (a.length() == 1 && ((a[0] - '0') == 0 || (a[0] - '0') == 1))
    //   return "1";

    // return mul(a, fact(sub(a, "1")));
    if (a.length() == 1 && ((a[0] - '0') == 0 || (a[0] - '0') == 1)) return "1";
    string fact = "1";
    int size = a.length();
    while (true) {
        if (a.length() == 1 && ((a[0] - '0') == 0 || (a[0] - '0') == 1)) {
            break;
        }
        fact = mul(fact, a);
        // cout<<fact<<endl;
        a = sub(a, "1");
        // cout<<a<<endl;
    }
    return fact;
}

string exp(string a, long long b) {
    if (b == 0) return "1";
    if (b % 2)
        return mul(a, mul(exp(a, b / 2), exp(a, b / 2)));
    else
        return mul(exp(a, b / 2), exp(a, b / 2));
}

int pre(char a) {
    if (a == 'x') return 2;
    if (a == '+' || a == '-') return 1;
    return 0;
}
string op(string a, string b, char opr) {
    a = removeZeroS(a);
    b = removeZeroS(b);
    if (opr == '+') return add(a, b);
    if (opr == '-') return sub(a, b);
    if (opr == 'x') return mul(a, b);
}

string solve(string a) {
    long long n = a.size();
    char stk1[n];

    int top1 = -1;
    int top2 = -1;

    string stk2[n];

    long long i = 0;

    string temp = "";

    while (i < n) {
        if (a[i] != '+' && a[i] != '-' && a[i] != 'x') {
            while (i < n && (a[i] != '+' && a[i] != '-' && a[i] != 'x')) {
                temp += a[i];
                i++;
            }
            top2++;
            stk2[top2] = temp;
            // cout<<temp<<endl;
            temp = "";
            // cout<<a[i]<<endl;
            i--;
        } else {
            while (top1 > -1 && pre(stk1[top1]) >= pre(a[i])) {
                string x = stk2[top2--];
                string y = stk2[top2--];
                char opr = stk1[top1--];
                top2++;
                stk2[top2] = op(x, y, opr);
            }
            stk1[++top1] = a[i];
        }
        i++;
    }
    while (top1 > -1) {
        string x = stk2[top2--];
        string y = stk2[top2--];
        char opr = stk1[top1--];

        stk2[++top2] = op(y, x, opr);
    }

    return stk2[top2];
}

string mod(string a, string b) {
    if (b == "1") return "0";
    if (isgreater(b, a)) return a;
    int i = 0;
    string m = "0";
    string temp = "";

    while (i < a.length()) {
        if (isgreater(temp, b)) {
            while (isgreater(temp, b)) {
                temp = sub(temp, b);
                temp = removeZeroS(temp);
            }
            m = temp;
        }
        // while(isgreater(b,temp))
        // {  // cout<<temp<<endl;
        temp += a[i];
        i += 1;
        if (i == a.length() && isgreater(b, temp)) return temp;

        // }
        // if()
    }
    if (isgreater(temp, b)) {
        while (isgreater(temp, b)) {
            temp = sub(temp, b);
            temp = removeZeroS(temp);
        }
        m = temp;
    }
    return m;

    // while(isgreater(a,b)){
    //   if(isgreater(curr,a))
    //   {a = sub(a,prev);
    //     curr = b;
    //     prev = "";
    //     }
    //   if(isequal(a,curr))
    //   return "0";
    //   else
    //   {prev = curr;
    //   curr = mul(curr,"2"); }
    // }
    // return a;
}

string gcd(string a, string b) {
    a = removeZeroS(a);
    b = removeZeroS(b);
    if (mod(a, b) == "0")
        return b;
    else
        return gcd(b, mod(a, b));
}

int main() {
    while (true) {
        int c;
        cin >> c;

        if (c == 1) {
            string a = "";
            cin >> a;
            cout << solve(a) << endl;
        } else if (c == 2) {
            string a = "";
            cin >> a;
            long long b;
            cin >> b;
            cout << exp(a, b) << endl;

        } else if (c == 3) {
            string a = "", b = "";
            cin >> a;
            cin >> b;
            cout << gcd(a, b) << endl;

        } else if (c == 4) {
            string a = "";
            cin >> a;
            cout << fact(a) << endl;
        } else {
            break;
        }
    }
    return 0;
}