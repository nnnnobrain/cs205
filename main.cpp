#include <stdio.h>
#include "WTNumber.h"
#include <string.h>
#include<string>
#include<algorithm>
#include<iostream>
#include<stack>
#include <utility>
#include <vector>
#include<cstring>
#include<cmath>
#include<random>

using namespace std;

void TreatVar(string line);

static void CinDeal(string line);

vector<string> TranferBack(string line);

int Priority(char sign);

string DelSpace(string line);

string CalculateBack(vector<string> line);

string MathFunc(string line);

string VarSimplify(string line);

bool isAssign(const string &line);

string ReplaceVar(string line);

bool IsContained(string line, string func);

bool IsStillContained(string line);

bool isBracketMatching(string line);

int IsVarExist(const string &var);

string ValSimplify(string line);

bool IsNotDigit(const char *num);

char *SNToNum(char *num1, int length1);

char *SignNum(char *num, int count, int length);

string pretreat(string line);

vector<string> varList;
vector<string> valList;

int main() {
    cout << "Welcome using this simple calculator!" << endl;
    cout<<"If you need introduction,print help"<<endl;
    while (true) {
        string line;
        getline(cin, line);
//        cout << line << endl;
        if (line == "quit") {
            cout << "Are you sure to quit? Print[y] to exit,otherwise will return" << endl;
            string nline;
            getline(cin, nline);
            if (nline == "y") {
                break;
            } else {
                continue;
            }
        }
        if (line == "help") {
            cout
                    << "This calculator program support sqrt(x), pow(a,b), max(a,b),abs(a),exp(a),min(a,b),sin(x),cos(x),tan(x),log(x)"
                    << endl;
            cout << "The calculator supports calculations with infinite precision" << endl;
            cout
                    << "The calculator supports the basic mathematics of addition, subtraction, multiplication and division with parentheses"
                    << endl;
            cout << "Variables can be defined" << endl;
            cout << "If you want to exit, please print quit" << endl;
            continue;
        }
        if (line == "") {
            continue;
        }
        string deledLine = DelSpace(line);
        deledLine = pretreat(deledLine);
        if (!isBracketMatching(deledLine)) {
            cout << "The brackets are not matched" << endl;
            return 0;
        }
        try {
            if (isAssign(deledLine)) {
                TreatVar(deledLine);
                continue;
            }
            string newline = MathFunc(deledLine);
            newline = ReplaceVar(newline);
            vector<string> backline = TranferBack(newline);
            string n1 = CalculateBack(backline);
            cout << n1 << endl;
        } catch (const char *msg) {
            cout << msg << endl;
        }
    }
    return 0;
}

string pretreat(string line) {
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == '-') {
            if (i == 0) {
                line = '0' + line;
            }
            if (line[i - 1] == '(' && i - 1 >= 0) {
                string front = line.substr(0, i);
                string back = line.substr(i, line.size() - i);
                line = line.substr(0, i) + '0' + line.substr(i, line.size() - i);
            }
        }
    }
    return line;
}

void CinDeal(string line) {
    vector<string> newline = TranferBack(line);
    for (int i = 0; i < newline.size(); ++i) {
        cout << newline.at(i);
    }
    cout << endl;
    CalculateBack(newline);
}

vector<string> TranferBack(string ogline) {
    vector<string> changedLine;
    stack<char> sign;
    string line = DelSpace(ogline);
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] >= '0' && line[i] <= '9') {
            string temp;
            temp += line[i];
            for (; (line[i + 1] >= '0' && line[i + 1] <= '9' && i + 1 < line.size()) || line[i + 1] == '.' ||
                   line[i + 1] == 'e'; ++i) {
                temp += line[i + 1];
            }
            changedLine.push_back(temp);
        } else if (line[i] == '+' || line[i] == '-' || line[i] == '/' || line[i] == '*' || line[i] == '(' ||
                   line[i] == ')') {
            if (sign.empty()) {
                sign.push(line[i]);
            } else {
                while (Priority(line[i]) <= Priority(sign.top()) && sign.top() != '(') {
                    if (line[i] == ')') {
                        while (sign.top() != '(') {
                            string temp(1, sign.top());
                            changedLine.push_back(temp);
                            sign.pop();
                        }
                        sign.pop();//把左括号弹出
                        break;
                    }
                    string temp(1, sign.top());
                    changedLine.push_back(temp);
                    sign.pop();
                    if (sign.empty()) {
                        break;
                    }
                }
                if (line[i] != ')') {
                    sign.push(line[i]);
                }
            }
        } else {
            changedLine.emplace_back("nan");
        }
    }
    while (!sign.empty()) {
        string temp(1, sign.top());
        changedLine.push_back(temp);
        sign.pop();
    }

    return changedLine;

}

string CalculateBack(vector<string> line) {
    stack<CWTNumber> num;
    CWTNumber c;
    if (line.size() == 1) {
        return CWTNumber(line[0].c_str()).ToString();
    }
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == "nan") {
            throw "Not an Number";
        }
        if (line[i][0] <= '9' && line[i][0] >= '0') {
            line[i] = SNToNum((char *) line[i].c_str(), line[i].size());
            if (IsNotDigit(line[i].c_str())) {
                throw "Not an Number";
            }
            num.push(CWTNumber(line[i].c_str()));
        } else {
            CWTNumber num2 = num.top();
            num.pop();
            CWTNumber num1 = num.top();
            num.pop();
            if (line[i] == "+") {
                c = num1 + num2;
                num.push(c);
            } else if (line[i] == "-") {
                c = num1 - num2;
                num.push(c);
            } else if (line[i] == "*") {
                c = num1 * num2;
                num.push(c);
            } else if (line[i] == "/") {
                c = num1 / num2;
                num.push(c);
            }
        }
    }
    return c.ToString();
}

string DelSpace(string line) {
    string changedline;
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] != ' ') {
            changedline += line[i];
        }
    }
    return changedline;
}

int Priority(char sign) {
    switch (sign) {
        case '+':
            return 0;
        case '-':
            return 0;
        case '*':
            return 1;
        case '/':
            return 1;
        case '(':
            return 5;
        case ')':
            return -1;

    }
}

void TreatVar(string line) {
    int equalIndex = line.find('=');//如果不是赋值会怎么样
    string var = line.substr(0, equalIndex);
    string val = line.substr(equalIndex + 1, line.size() - equalIndex - 1);
    val = ReplaceVar(val);
    val = ValSimplify(val);
    if (val == "nan") {
        throw "Not an Number";
    }
    int a = IsVarExist(var);
    if (a != -1) {
        valList.at(a) = val;
    } else {
        varList.push_back(var);
        valList.push_back(val);
    }

}

string ReplaceVar(string line) {
    for (int i = 0; i < varList.size(); ++i) {
        string temp = varList.at(i);
        for (int j = 0; j < line.size(); ++j) {
        int index = line.find(temp);
        if (index != -1) {
            string front = line.substr(0, index);
            string back = line.substr(index + 1, line.size() - index - 1);
            line = front + valList.at(i) + back;
        }
        if (index == -1 && (line[i] < 48 || line[i] > 57) &&
            (line[i] != '+' && line[i] != '-' && line[i] != '*' && line[i] != '/')) {
            throw "The variable is not existed";
        }
        }
    }
    return line;
}

int IsVarExist(const string &var) {
    int isExist = -1;
    for (int i = 0; i < varList.size(); ++i) {
        if (varList.at(i) == var) {
            isExist = i;
            break;
        }
    }
    return isExist;
}

bool isAssign(const string &line) {
    bool equal = false;
    bool varName = true;

    int equalIndex = line.find('=');
    string var = line.substr(0, equalIndex);
    if (equalIndex != -1) {
        equal = true;
    }
    if (isalpha(var[0])) {
        varName = true;
    }
    for (int i = 1; i < var.size(); ++i) {
        if (!isdigit(var[i]) || var[i] != '_' || !isalpha(var[i])) {
            varName = false;
            break;
        }
    }
    if (!varName && equal) {
        throw "The name of variable is invalid";
    }
    if (equal && varName) {
        return true;
    } else {
        return false;
    }
}

string VarSimplify(string line) {
    string replacedLine = ReplaceVar(line);
    vector<string> backLine = TranferBack(replacedLine);
    string res = CalculateBack(backLine);
    return res;
}

string ValSimplify(string line) {

    string replacedLine = MathFunc(line);
    vector<string> backLine = TranferBack(replacedLine);
    string res = CalculateBack(backLine);
    return res;
}

string MathFunc(string line) {

    int index = line.find("sqrt(");

    if (index != -1) {
        int right = 0;
        for (int i = index; i < line.size(); ++i) {
            if (line[i] == ')') {
                right = i;
                break;
            }
        }
        string front = line.substr(0, index);
        string mid = line.substr(index + strlen("sqrt("), right - (index + strlen("sqrt(")));
        string back = line.substr(right + 1, line.size() - right - 1);
        if (IsStillContained(mid)) {
            mid = MathFunc(mid);
        } else {
            mid = VarSimplify(mid);
        }
        if (stod(mid) < 0) {
            line = "Not an Number";
            return line;
        } else {
            mid = to_string(sqrt(stod(mid)));
            line = front + mid + back;
        }
    }

    //-----------------------------------------------------
    label1:
    index = line.find("exp(");
    if (index != -1) {
        int right = 0;
        for (int i = index; i < line.size(); ++i) {
            if (line[i] == ')') {
                right = i;
                break;
            }
        }
        string front = line.substr(0, index);
        string mid = line.substr(index + strlen("exp("), right - (index + strlen("exp(")));
        string back = line.substr(right + 1, line.size() - right - 1);
        if (IsStillContained(mid)) {
            mid = MathFunc(mid);
        } else {
            mid = VarSimplify(mid);
        }
        mid = to_string(exp(stod(mid)));
        line = front + mid + back;
    }
    //-----------------------------------------------------
    label2:
    index = line.find("abs(");
    if (index != -1) {
        int right = 0;
        for (int i = index; i < line.size(); ++i) {
            if (line[i] == ')') {
                right = i;
                break;
            }
        }
        string front = line.substr(0, index);
        string mid = line.substr(index + strlen("abs("), right - (index + strlen("abs(")));
        string back = line.substr(right + 1, line.size() - right - 1);
        if (IsStillContained(mid)) {
            mid = MathFunc(mid);
        } else {
            mid = VarSimplify(mid);
        }
        mid = to_string(abs(stod(mid)));
        line = front + mid + back;
    }
    //-------------------------------------------------
    label3:
    index = line.find("max(");
    if (index != -1) {
        int right = 0;
        int com = 0;
        for (int i = index; i < line.size(); ++i) {
            if (line[i] == (',')) {
                com = i;
            }
            if (line[i] == ')') {
                right = i;
            }
        }
        string front = line.substr(0, index);
        string num1 = line.substr(index + strlen("max("), com - (index + strlen("max(")));
        string num2 = line.substr(com + 1, right - com - 1);
        string back = line.substr(right + 1, line.size() - right - 1);
        if (IsStillContained(num1)) {
            num1 = MathFunc(num1);
        } else {
            num1 = VarSimplify(num1);
        }
        if (IsStillContained(num2)) {
            num2 = MathFunc(num2);
        } else {
            num2 = VarSimplify(num2);
        }
        string mid = to_string(max(stod(num1), stod(num2)));
        line = front + mid + back;
    }
    //--------------------------------------------------
    label4:
    index = line.find("min(");
    if (index != -1) {
        int right = 0;
        int com = 0;
        for (int i = index; i < line.size(); ++i) {
            if (line[i] == (',')) {
                com = i;
            }
            if (line[i] == ')') {
                right = i;
            }
        }
        string front = line.substr(0, index);
        string num1 = line.substr(index + strlen("min("), com - (index + strlen("min(")));
        string num2 = line.substr(com + 1, right - com - 1);
        string back = line.substr(right + 1, line.size() - right - 1);
        if (IsStillContained(num1)) {
            num1 = MathFunc(num1);
        } else {
            num1 = VarSimplify(num1);
        }
        if (IsStillContained(num2)) {
            num2 = MathFunc(num2);
        } else {
            num2 = VarSimplify(num2);
        }
        string mid = to_string(min(stod(num1), stod(num2)));
        line = front + mid + back;
    }
    //--------------------------------------------------------
    label5:
    index = line.find("sin(");
    if (index != -1) {
        int right = 0;
        for (int i = index; i < line.size(); ++i) {
            if (line[i] == ')') {
                right = i;
                break;
            }
        }
        string front = line.substr(0, index);
        string mid = line.substr(index + strlen("sin("), right - (index + strlen("sin(")));
        string back = line.substr(right + 1, line.size() - right - 1);
        if (IsStillContained(mid)) {
            mid = MathFunc(mid);
        } else {
            mid = VarSimplify(mid);
        }
        mid = to_string(sin(stod(mid)));
        line = front + mid + back;
    }
    //----------------------------------------------------------------
    label6:
    index = line.find("cos(");
    if (index != -1) {
        int right = 0;
        for (int i = index; i < line.size(); ++i) {
            if (line[i] == ')') {
                right = i;
                break;
            }
        }
        string front = line.substr(0, index);
        string mid = line.substr(index + strlen("cos("), right - (index + strlen("cos(")));
        string back = line.substr(right + 1, line.size() - right - 1);
        if (IsStillContained(mid)) {
            mid = MathFunc(mid);
        } else {
            mid = VarSimplify(mid);
        }
        mid = to_string(cos(stod(mid)));
        line = front + mid + back;
    }
    //-----------------------------------------------------------------------
    label7:

    //------------------
    label8:
    index = line.find("tan(");
    if (index != -1) {
        int right = 0;
        for (int i = index; i < line.size(); ++i) {
            if (line[i] == ')') {
                right = i;
                break;
            }
        }
        string front = line.substr(0, index);
        string mid = line.substr(index + strlen("tan("), right - (index + strlen("tan(")));
        string back = line.substr(right + 1, line.size() - right - 1);
        if (IsStillContained(mid)) {
            mid = MathFunc(mid);
        } else {
            mid = VarSimplify(mid);
        }
        mid = to_string(tan(stod(mid)));
        line = front + mid + back;
    }
    //------------------------------------------
    label9:
    index = line.find("pow(");
    if (index != -1) {
        int right = 0;
        int com = 0;
        for (int i = index; i < line.size(); ++i) {
            if (line[i] == (',')) {
                com = i;
            }
            if (line[i] == ')') {
                right = i;
            }
        }
        string front = line.substr(0, index);
        string num1 = line.substr(index + strlen("pow("), com - (index + strlen("pow(")));
        string num2 = line.substr(com + 1, right - com - 1);
        string back = line.substr(right + 1, line.size() - right - 1);
        if (IsStillContained(num1)) {
            num1 = MathFunc(num1);
        } else {
            num1 = VarSimplify(num1);
        }
        if (IsStillContained(num2)) {
            num2 = MathFunc(num2);
        } else {
            num2 = VarSimplify(num2);
        }
        string mid = to_string(pow(stod(num1), stod(num2)));
        line = front + mid + back;
    }
    //-------------------------------------------------------
    label10:
    index = line.find("log(");
    if (index != -1) {
        int right = 0;
        for (int i = index; i < line.size(); ++i) {
            if (line[i] == ')') {
                right = i;
                break;
            }
        }
        string front = line.substr(0, index);
        string mid = line.substr(index + strlen("log("), right - (index + strlen("log(")));
        string back = line.substr(right + 1, line.size() - right - 1);
        if (IsStillContained(mid)) {
            mid = MathFunc(mid);
        } else {
            mid = VarSimplify(mid);
        }
        mid = to_string(log(stod(mid)));
        line = front + mid + back;
    }

    return line;
}

bool IsStillContained(string line) {
    int index1 = line.find("sqrt(");
    int index2 = line.find("cos(");
    int index3 = line.find("sin(");
    int index4 = line.find("tan(");
    int index5 = line.find("log(");
    int index6 = line.find("pow(");
    int index7 = line.find("max(");
    int index8 = line.find("min(");
    int index9 = line.find("exp(");
    int index10 = line.find("abs(");
    if (index1 != -1 || index2 != -1 || index3 != -1 || index4 != -1 || index5 != -1 || index6 != -1 || index7 != -1 ||
        index8 != -1 || index9 != -1 || index10 != -1 ) {
        return true;
    } else {
        return false;
    }
}

bool isBracketMatching(string line) {
    int leftCount = 0;
    int rightCount = 0;
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == '(') {
            leftCount++;
        }
        if (line[i] == ')') {
            rightCount++;
        }
    }
    if (leftCount == rightCount) {
        return true;
    } else {
        return false;
    }
}

char *SNToNum(char *num1, int length1) {
//    int length = sizeof(num) / sizeof(char);
    int count = 0;
//    int length = 7;
    int floatcount = 0;
    for (int i = 0; i < length1; ++i) {
        if (num1[i] == 'E' || num1[i] == 'e') {
            count = i;
        }
        if (num1[i] == '.') {
            floatcount = i;
        }
    }

    char *num = SignNum(num1, count, length1);
    int length = strlen(num);
    if (count != 0 && (num[count + 1] != '+' && num[count + 1] != '-')) {
        return num;
    }
    if (count != 0) {
        int frontlength = 0;
        if (floatcount != 0) {//浮点数的情况
            frontlength = count - 1;

            char front[frontlength];
            for (int i = 0, j = 0; i < count; ++i, j++) {
                if (!isdigit(num[i]) && num[i] != '.') {
                    return num;
                }
                if (num[i] == '.') {
                    i++;
                }
                front[j] = num[i];
            }
            char back[length - count - 2];
            for (int i = count + 2, j = 0; i < length; ++i, j++) {
                if (!isdigit(num[i])) {
                    return num;
                }
                back[j] = num[i];
            }
            for (int i = 0; i < frontlength; ++i) {//改动 frontlength
                if (!isdigit(front[i])) {
                    return num;
                }

            }
            double frontnum = strtod(front, NULL);
            for (int i = 0; i < length - count - 3; ++i) {
                if (!isdigit(back[i])) {
                    return num;
                }
            }
            int backnum = stoi(back, nullptr);
            char res[count + backnum];
            if (num[count + 1] == '+') {
                for (int i = 0, n = 0; i < frontlength + backnum; ++i, n++) {//改动 frontlength
                    if (n == (count + backnum) - (count - floatcount)) {
                        res[i] = '.';
                        i--;
                        continue;
                    }
                    if (i < frontlength) {//改动 frontlength
                        res[i] = front[i];
                    } else {
                        res[n] = '0';
                    }

                }
            } else if (num[count + 1] == '-') {//此处为负号
                for (int i = 0, n = 0; i < frontlength + backnum; ++i, n++) {//改动 frontlength
                    if (n == 1) {
                        res[i] = '.';
                        i--;
                        continue;
                    }
                    if (i < backnum) {
                        res[n] = '0';
                    } else {
                        res[n] = front[i - backnum];
                    }

                }
            }
            char *ret = (char *) malloc(sizeof(char) * (count + backnum + 1));
            memset(ret, 0, sizeof(char) * (count + backnum + 1));
            for (int i = 0; i < count + backnum + 1; ++i) {
                ret[i] = res[i];

                if (i == count + backnum) {
                    ret[i] = '\0';
                }
            }
            // cout << ret << endl;
            return ret;
        } else {//整数的情况
            frontlength = count;
            char front[frontlength];
            for (int i = 0, j = 0; i < count; ++i, j++) {
                if (!isdigit(num[i])) {
                    return num;
                }
                front[j] = num[i];
            }
            char back[length - count - 2];
            for (int i = count + 2, j = 0; i < length; ++i, j++) {
                if (!isdigit(num[i])) {
                    return num;
                }
                back[j] = num[i];
            }
            for (int i = 0; i < frontlength; ++i) {//改动 frontlength
                if (!isdigit(front[i])) {
                    return num;
                }

            }
            double frontnum = strtod(front, NULL);
            for (int i = 0; i < length - count - 3; ++i) {
                if (!isdigit(back[i])) {
                    return num;
                }
            }
            int backnum = stoi(back, nullptr);
            char *ret = (char *) malloc(sizeof(char) * (count + backnum + 1));
            memset(ret, 0, sizeof(char) * (count + backnum + 1));
            if (num[count + 1] == '+') {
                char res[count + backnum];
                for (int i = 0, n = 0; i < frontlength + backnum; ++i, n++) {//改动 frontlength
                    if (i < frontlength) {//改动 frontlength
                        res[i] = front[i];
                    } else {
                        res[i] = '0';
                    }
                }
                for (int i = 0; i < count + backnum + 1; ++i) {
                    ret[i] = res[i];
                    if (i == count + backnum) {
                        ret[i] = '\0';
                    }
                }
                // cout << ret << endl;
            } else if (num[count + 1] == '-') {//此处为负号
                char res[count + backnum + 1];
                memset(ret, 0, sizeof(char) * (count + backnum + 1 + 1));
                for (int i = 0, n = 0; i < frontlength + backnum; ++i, n++) {//改动 frontlength
                    if (n == 1) {
                        res[i] = '.';
                        i--;
                        continue;
                    }
                    if (i < backnum) {
                        res[n] = '0';
                    } else {
                        res[n] = front[i - backnum];
                    }
                }
                for (int i = 0; i < count + backnum + 1 + 1; ++i) {
                    ret[i] = res[i];
                    if (i == count + backnum + 1) {
                        ret[i] = '\0';
                    }
                }
                // cout << ret << endl;
            }

            return ret;
        }

    } else {
        return num;
    }

}

bool IsNotDigit(const char *num) {
    int length = strlen(num);
    bool IsNotDigit = false;
    for (int i = 0; i < length; ++i) {
        if ((num[i] < 48 || num[i] > 57) && num[i] != '.') {
            return true;
        }
    }
    return IsNotDigit;

}

char *SignNum(char *num, int count, int length) {

    bool havenotsign = num[count + 1] > 48 && num[count + 1] <= 57;
    if (havenotsign) {
        length++;
    }
    char *signedNum = (char *) malloc(sizeof(char) * (length + 1));
    memset(signedNum, 0, sizeof(char) * (length + 1));
    if (havenotsign && count != 0) {
        for (int i = 0, n = 0; i < length + 1; ++i, n++) {
            if (n == count + 1) {
                signedNum[i] = '+';
                i--;
                continue;
            }
            signedNum[n] = num[i];

        }
    } else {
        return (char *) num;
    }
    return signedNum;
}
