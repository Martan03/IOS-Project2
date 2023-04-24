#include <stdio.h> // fprintf, stderr
#include <stdbool.h> // bool
#include <inttypes.h> // SIZE_MAX
#include <ctype.h> // isspace
#include <stdlib.h> // strtoul

#define MAX_NZ SIZE_MAX
#define MAX_NU SIZE_MAX
#define MAX_TZ ((size_t) 10000)
#define MAX_TU ((size_t) 100)
#define MAX_F ((size_t) 10000)

/// @brief Parses number from string to size_t, checks max value
/// @param str number in string
/// @param out output size_t variable
/// @param max max value of the number
/// @return true on success, else false
bool parse_number(char* str, size_t* out, size_t max);

int main(int argc, char** argv) {
    // Checks if all arguments were given
    if (argc != 6) {
        fprintf(stderr, "Error: invalid usage.\n");
        return 1;
    }

    size_t nz, nu, tz, tu, f;

    // Checks if arguments are numbers in valid range
    if (!parse_number(argv[1], &nz, MAX_NZ)) {
        fprintf(stderr, "Error: NZ must be an integer.\n");
        return 1;
    }
    if (!parse_number(argv[1], &nu, MAX_NU)) {
        fprintf(stderr, "Error: NU must be an integer.\n");
        return 1;
    }
    if (!parse_number(argv[1], &tz, MAX_TZ)) {
        fprintf(stderr, "Error: TZ must be integer less than %zu.\n", MAX_TZ);
        return 1;
    }
    if (!parse_number(argv[1], &tu, MAX_TU)) {
        fprintf(stderr, "Error: TZ must be integer less than %zu.\n", MAX_TU);
        return 1;
    }
    if (!parse_number(argv[1], &f, MAX_F)) {
        fprintf(stderr, "Error: TZ must be integer less than %zu.\n", MAX_F);
        return 1;
    }

    return 0;
}

bool parse_number(char* str, size_t* out, size_t max) {
    // Removes whitespaces from beginning of the string
    while (isspace(*str))
        ++str;

    if (*str == 0 || str[0] == '-')
        return false;

    // Converts string to size_t
    *out = strtoul(str, &str, 10);

    return *str == 0 && *out <= max;
}
