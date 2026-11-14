// ATX headings: rewrite heading lines, echo every other line.
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

// ---- given: read stdin as lines, write lines to stdout ----
static std::vector<std::string> read_lines() {
    std::string raw((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());
    std::string text;
    for (size_t i = 0; i < raw.size(); i++) {
        if (raw[i] == '\r' && i + 1 < raw.size() && raw[i + 1] == '\n') continue;
        text += raw[i];
    }
    if (!text.empty() && text[text.size() - 1] == '\n') text.erase(text.size() - 1);
    std::vector<std::string> lines;
    std::string cur;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '\n') { lines.push_back(cur); cur.clear(); }
        else cur += text[i];
    }
    lines.push_back(cur);
    return lines;
}

static void write_lines(const std::vector<std::string> &out) {
    std::string s;
    for (size_t i = 0; i < out.size(); i++) { s += out[i]; s += '\n'; }
    if (s.empty()) s = "\n";
    std::cout << s;
}
// ---- end of given code ----

static bool is_ws(char c) { return c == ' ' || c == '\t'; }

// Return the HTML for one line: a heading tag, or the line unchanged.
static std::string heading(const std::string &line) {
    // TODO: 0-3 spaces, then 1-6 '#', then a space, a tab or the end of the line.
    // TODO: trim the text, drop a closing run of '#' that follows a space or tab.
    // TODO: anything that is not a heading comes back unchanged.
    return line;
}

int main() {
    std::vector<std::string> lines = read_lines();
    for (size_t i = 0; i < lines.size(); i++) lines[i] = heading(lines[i]);
    write_lines(lines);
    return 0;
}
