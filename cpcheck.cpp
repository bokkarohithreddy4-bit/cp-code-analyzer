#include <bits/stdc++.h>
using namespace std;

// Stores info about one function in the file.
struct FunctionInfo {
    string name;
    int startLine;   // line where function header starts (1-based)
    int endLine;     // line where function ends (1-based)
    int length;      // number of lines in the function (header + body)
    int maxDepth;    // maximum nesting depth inside this function
};

// Trim spaces from both ends of a string.
string trim(const string &s) {
    int i = 0, j = (int)s.size() - 1;
    while (i <= j && isspace((unsigned char)s[i])) i++;
    while (j >= i && isspace((unsigned char)s[j])) j--;
    if (i > j) return "";
    return s.substr(i, j - i + 1);
}

// Very naive check: does this line look like a function header?
bool isFunctionHeader(const string &line) {
    string s = trim(line);
    if (s.empty()) return false;
    if (s[0] == '/' || s[0] == '#') return false; // comment or include

    // Must contain '(' and ')' and end with '{'
    size_t posOpen  = s.find('(');
    size_t posClose = s.find(')');
    if (posOpen == string::npos || posClose == string::npos) return false;
    if (posClose < posOpen) return false;
    if (s.back() != '{') return false;

    // Avoid if/for/while/switch blocks
    if (s.rfind("if", 0) == 0 ||
        s.rfind("for", 0) == 0 ||
        s.rfind("while", 0) == 0 ||
        s.rfind("switch", 0) == 0)
        return false;

    return true;
}

// Try to extract function name from header line.
string getFunctionName(const string &line) {
    string s = trim(line);
    size_t posOpen = s.find('(');
    if (posOpen == string::npos) return "unknown";

    int j = (int)posOpen - 1;
    while (j >= 0 && (isalnum((unsigned char)s[j]) || s[j] == '_')) j--;
    int start = j + 1;
    if (start < 0 || start >= (int)s.size()) return "unknown";
    return s.substr(start, posOpen - start);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: cpcheck <file.cpp>\n";
        return 0;
    }

    string path = argv[1];
    ifstream in(path);
    if (!in) {
        cout << "Could not open file: " << path << "\n";
        return 0;
    }

    // Read all lines of the file.
    vector<string> lines;
    string line;
    while (getline(in, line)) {
        lines.push_back(line);
    }

    int totalLines = (int)lines.size();
    cout << "Analyzing file: " << path << "\n";
    cout << "Total lines: " << totalLines << "\n";

    // Extra check: count lines containing loops (for/while).
    int loopLines = 0;
    for (const string &ln : lines) {
        if (ln.find("for") != string::npos ||
            ln.find("while") != string::npos) {
            loopLines++;
        }
    }
    cout << "Lines containing loops (for/while): " << loopLines << "\n";

    const int MAX_FUNCTION_LENGTH = 60;
    const int MAX_NESTING_DEPTH   = 3;

    vector<FunctionInfo> functions;

    int n = totalLines;
    int i = 0;
    while (i < n) {
        if (!isFunctionHeader(lines[i])) {
            i++;
            continue;
        }

        string funcName = getFunctionName(lines[i]);
        int headerLine = i + 1; // 1-based

        // Track braces to find end of function.
        int braceDepth = 0;
        // Count braces on header line itself.
        for (char c : lines[i]) {
            if (c == '{') braceDepth++;
            if (c == '}') braceDepth--;
        }

        int j = i + 1;
        int currentDepth = braceDepth;
        int maxDepth = braceDepth;

        while (j < n && braceDepth > 0) {
            for (char c : lines[j]) {
                if (c == '{') {
                    braceDepth++;
                    currentDepth++;
                    maxDepth = max(maxDepth, currentDepth);
                } else if (c == '}') {
                    braceDepth--;
                    currentDepth--;
                }
            }
            j++;
        }

        int endLine = j;           // 1-based
        int length  = endLine - i; // include header line

        FunctionInfo info;
        info.name      = funcName;
        info.startLine = headerLine;
        info.endLine   = endLine;
        info.length    = length;
        info.maxDepth  = maxDepth;

        functions.push_back(info);

        i = j; // move to next after this function
    }

    if (functions.empty()) {
        cout << "No functions detected (very naive parser).\n";
        return 0;
    }

    cout << "\nDetected functions:\n";
    for (const auto &f : functions) {
        cout << "  " << f.name
             << " (lines " << f.startLine << "-" << f.endLine << ")"
             << ", length = " << f.length
             << ", maxDepth = " << f.maxDepth << "\n";
    }

    cout << "\nWarnings:\n";
    bool anyWarning = false;
    for (const auto &f : functions) {
        if (f.length > MAX_FUNCTION_LENGTH) {
            anyWarning = true;
            cout << "  Function '" << f.name << "' is long ("
                 << f.length << " lines, limit " << MAX_FUNCTION_LENGTH << ").\n";
        }
        if (f.maxDepth > MAX_NESTING_DEPTH) {
            anyWarning = true;
            cout << "  Function '" << f.name << "' has high nesting depth ("
                 << f.maxDepth << ", limit " << MAX_NESTING_DEPTH << ").\n";
        }
    }

    if (!anyWarning) {
        cout << "  No warnings.\n";
    }

    return 0;
}
