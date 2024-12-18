#ifndef UTILITY_H
#define UTILITY_H

#include <string>

int getInt(const std::string& prompt);
float getFloat(const std::string& prompt);
double getDouble(const std::string& prompt);
std::string getString(const std::string& prompt);
std::string getDate(const std::string& prompt);
void copyStr(const std::string &str, char* c_str, size_t size);
void printCategory();

#endif // UTILITY H