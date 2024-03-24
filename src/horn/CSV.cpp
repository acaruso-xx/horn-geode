#include "CSV.hpp"

#include <Geode/utils/string.hpp>

namespace horn {

std::vector<CSV::Row> CSV::rows() {
    using namespace geode::utils;
    std::vector<Row> rows;

    for (auto const& row : string::split(m_str, "\n")) {
        rows.push_back(parse(row));
    }

    return rows;
}

CSV::Row CSV::parse(std::string const& str) {
    m_state = State::UnquotedField;
    m_fields = { "" };
    m_index = 0;

    for (auto c : str) {
        switch (m_state) {
        case State::UnquotedField:
            state_unquotedField(c);
            break;
        case State::QuotedField:
            state_quotedField(c);
            break;
        case State::QuotedQuote:
            state_quotedQuote(c);
            break;
        };
    }

    return m_fields;
}

void CSV::state_unquotedField(char c) {
    switch (c) {
    case ',':
        m_fields.push_back("");
        m_index++;
        break;

    case '"':
        m_state = State::QuotedField;
        break;

    default:
        m_fields[m_index].push_back(c);
        break;
    }
}

void CSV::state_quotedField(char c) {
    switch (c) {
    case '"':
        m_state = State::QuotedQuote;
        break;

    default:
        m_fields[m_index].push_back(c);
        break;
    }
}

void CSV::state_quotedQuote(char c) {
    switch (c) {
    case ',':
        m_fields.push_back("");
        m_index++;
        m_state = State::UnquotedField;
        break;

    case '"':
        m_fields[m_index].push_back('"');
        m_state = State::QuotedField;
        break;

    default:
        m_state = State::UnquotedField;
        break;
    }
}

} // namespace horn
