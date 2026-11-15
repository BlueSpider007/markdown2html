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

// para holds the raw lines of one paragraph; return the text of its <p> block.
static std::string paragraph(const std::vector<std::string> &para) {
    std::string out = "<p>";
    for (size_t idx = 0; idx < para.size(); idx++) {
        std::string text = para[idx];
        size_t start = 0;
        while (start < text.size() && is_ws(text[start]))
            start++;
        text = text.substr(start);
        bool more = idx + 1 < para.size();
        bool hard = false;
        size_t n = text.size();
        if (more && n >= 2 && text[n - 1] == ' ' && text[n - 2] == ' ') {
            hard = true;
        } else if (more && n >= 1 && text[n - 1] == '\\') {
            hard = true;
            text.erase(n - 1);
        }
        size_t end = text.size();
        while (end > 0 && is_ws(text[end - 1]))
            end--;
        out += text.substr(0, end);
        if (more)
            out += hard ? "<br />\n" : " ";
    }
    out += "</p>";
    return out;
}

// Return the output blocks for these input lines.
static std::vector<std::string> render(const std::vector<std::string> &lines) {
    std::vector<std::string> out;
    std::vector<std::string> para;

    for (const std::string &line : lines) {
        bool blank = true;

        for (char c : line) {
            if (!is_ws(c)) {
                blank = false;
                break;
            }
        }

        if (blank) {
            if (!para.empty()) {
                out.push_back(paragraph(para));
                para.clear();
            }
        } else {
            para.push_back(line);
        }
    }
    if (!para.empty()) {
        out.push_back(paragraph(para));
    }
    return out;
}

int main() {
    write_lines(render(read_lines()));
    return 0;
}
