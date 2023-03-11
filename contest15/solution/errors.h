enum
{
    SUCCESS = 0,
    E_NO_NEWLINE = 1,
    E_WORD_EXPECTED_REDIRECT = 2,
    E_CLOSE_EXPECTED = 3,
    E_WORD_OR_OPEN_EXPECTED = 4,
    ENUM_ERRORS_END = 5
};

const char *error_message(int error);