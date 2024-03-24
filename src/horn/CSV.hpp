#pragma once

#include <string>
#include <vector>

namespace horn {

//! @brief Excel-style CSV reader.
class CSV final {
public:
    using Row = std::vector<std::string>;

    CSV(std::string const& str) : m_str(str) {}

    //! @brief Parse rows.
    //! @return Vector of parsed rows.
    std::vector<Row> rows();

private:
    enum class State {
        UnquotedField,
        QuotedField,
        QuotedQuote,
    };

    //! @brief Parse a row.
    //! @param str Unparsed row.
    //! @return Parsed row.
    Row parse(std::string const& str);

    //! @brief Handler for unquoted field state.
    //! @param c Character to consume.
    void state_unquotedField(char c);

    //! @brief Handler for unquoted field state.
    //! @param c Character to consume.
    void state_quotedField(char c);

    //! @brief Handler for unquoted field state.
    //! @param c Character to consume.
    void state_quotedQuote(char c);

    std::string const& m_str;

    State m_state;
    Row m_fields;
    std::size_t m_index;
};

} // namespace horn
