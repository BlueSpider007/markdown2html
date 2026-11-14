// ATX headings: rewrite heading lines, echo every other line.
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

// ---- given: read stdin as lines, write lines to stdout ----
static std::vector<std::string> read_lines() {
    std::string raw((std::istreambuf_iterator<char>(std::cin)),
                    std::istreambuf_iterator<char>());
    std::string text;
    for (size_t i = 0; i < raw.size(); i++) {
        if (raw[i] == '\r' && i + 1 < raw.size() && raw[i + 1] == '\n')
            continue;
        text += raw[i];
    }
    if (!text.empty() && text[text.size() - 1] == '\n')
        text.erase(text.size() - 1);
    std::vector<std::string> lines;
    std::string cur;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '\n') {
            lines.push_back(cur);
            cur.clear();
        } else
            cur += text[i];
    }
    lines.push_back(cur);
    return lines;
}

static void write_lines(const std::vector<std::string> &out) {
    std::string s;
    for (size_t i = 0; i < out.size(); i++) {
        s += out[i];
        s += '\n';
    }
    if (s.empty())
        s = "\n";
    std::cout << s;
}
// ---- end of given code ----

static bool is_ws(char c) { return c == ' ' || c == '\t'; }

// Return the HTML for one line: a heading tag, or the line unchanged.
static std::string heading(const std::string &line) {
    size_t pos = 0;
    while (pos < line.size() && pos < 3 && is_ws(line[pos])) {
        ++pos;
    }
    size_t start = pos;
    while (pos < line.size() && line[pos] == '#') {
        ++pos;
    }
    size_t level = pos - start;

    if (level < 1 || level > 6) {
        return line;
    }
    if (pos < line.size() && !is_ws(line[pos])) {
        return line;
    }

    std::string text = line.substr(pos);
    size_t first = text.find_first_not_of(" \t");
    if (first == std::string::npos) {
        text.clear();
    } else {
        size_t last = text.find_last_not_of(" \t");
        text = text.substr(first, last - first + 1);
    }

    size_t end = text.size();
    while (end > 0 && text[end - 1] == '#') {
        --end;
    }
    if (end < text.size() && end > 0 && is_ws(text[end - 1])) {
        text = text.substr(0, end);
    }

    if (!text.empty() && text.find_first_not_of('#') == std::string::npos) {
        text.clear();
    }

    while (!text.empty() && is_ws(text.back())) {
        text.pop_back();
    }
    return "<h" + std::to_string(level) + ">" + text + "</h" +
           std::to_string(level) + ">";
}

int main() {
    std::vector<std::string> lines = read_lines();
    for (size_t i = 0; i < lines.size(); i++)
        lines[i] = heading(lines[i]);
    write_lines(lines);
    return 0;
}
