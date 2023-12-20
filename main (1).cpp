#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<string, int> variables;

bool isIdentifier(const string &str) {
  if (!isalpha(str[0]) && str[0] != '_')
    return false;

  for (char c : str) {
    if (!isalnum(c) && c != '_')
      return false;
  }

  return true;
}

bool isLiteral(const string &str) {
  for (char c : str) {
    if (!isdigit(c))
      return false;
  }

  return true;
}

int evaluateExpression(const string &exp) {
  istringstream iss(exp);
  int result = 0;

  while (iss) {
    int term;
    iss >> term;

    if (!iss)
      break;

    result += term;
  }

  return result;
}

int evaluateFactor(const string &factor) {
  if (factor[0] == '(' && factor[factor.size() - 1] == ')') {
    return evaluateExpression(factor.substr(1, factor.size() - 2));
  } else if (factor[0] == '-') {
    return -evaluateFactor(factor.substr(1));
  } else if (factor[0] == '+') {
    return evaluateFactor(factor.substr(1));
  } else if (isLiteral(factor)) {
    return atoi(factor.c_str());
  } else if (isIdentifier(factor)) {
    if (variables.find(factor) != variables.end()) {
      return variables[factor];
    } else {
      cout << "Error: Variable '" << factor << "' is uninitialized.\n";
      exit(1);
    }
  } else {
    cout << "Syntax Error: Invalid factor '" << factor << "'.\n";
    exit(1);
  }
}

int evaluateTerm(const string &term) {
  istringstream iss(term);
  int result = evaluateFactor("");
  char op;

  while (iss >> op) {
    int nextFactor = evaluateFactor("");

    if (op == '*')
      result *= nextFactor;
    else if (op == '/')
      result /= nextFactor;
    else {
      cout << "Syntax Error: Invalid operator '" << op << "'.\n";
      exit(1);
    }
  }

  return result;
}


void processAssignment(const string& assignment) {
    size_t equalPos = assignment.find('=');
    if (equalPos == string::npos) {
        cout << "Syntax Error: Missing '=' in assignment.\n";
        exit(1);
    }

    string identifier = assignment.substr(0, equalPos);
    string expression = assignment.substr(equalPos + 1);

    if (!isIdentifier(identifier)) {
        cout << "Syntax Error: Invalid identifier '" << identifier << "'.\n";
        exit(1);
    }

    if (variables.find(identifier) != variables.end()) {
        cout << "Error: Variable '" << identifier << "' already assigned.\n";
        exit(1);
    }

    int result = evaluateExpression(expression);
    variables[identifier] = result;
}


void interpretProgram(const string &program) {
  istringstream iss(program);
  string line;

  while (getline(iss, line, ';')) {
    processAssignment(line + ';');
    if (iss.peek() != EOF && iss.peek() != '\n') {
      cout << "Syntax Error: Extra characters after assignment.\n";
      exit(1);
    }
  }
}

int main() {
  string program;
  cout << "Please Enter Your Program:\n";
  getline(cin, program);

  interpretProgram(program);

  // Print variable values
  for (const auto &entry : variables) {
    cout << entry.first << " = " << entry.second << endl;
  }

  return 0;
}