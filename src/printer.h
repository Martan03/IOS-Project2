// printer.h
// Řešení IOS - Projekt 2
// Autor: Martin Slezák, FIT
// Deklarace pomocných printovacích funkcí

#ifndef PRINTER_INCLUDED
#define PRINTER_INCLUDED

/// @brief Prints error message to stderr
/// @param fmt string to be printed
/// @param variables to insert to fmt string
/// @return true
int perr(const char* fmt, ...);

/// @brief Prints log message to the file
/// @param fmt string to be printed
/// @param variables to insert to fmt string
void plog(const char* fmt, ...);

/// @brief Opens file for printing log
/// @return 0 on success, else 1
int open_log_file(void);

/// @brief Closes log file
void close_log_file(void);

#endif // PRINTER_INCLUDED
