#ifndef UTILITY_H
#define UTILITY_H

#include <string>

int getInt(const std::string& prompt);
float getFloat(const std::string& prompt);
double getDouble(const std::string& prompt);
std::string getString(const std::string& prompt);
int getId(const std::string& prompt, const std::string& tableName);

#endif // UTILITY H