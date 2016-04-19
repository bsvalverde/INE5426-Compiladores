#include <map>

class Symtable {
public:
	static Symtable* getInstance();
	void addVar(const char* name, int value);
	int getVar(const char* name);
	void setVar(const char* name, int newValue);
	bool hasVar(const char* name);
private:
	Symtable();

	static Symtable* instance;
	std::map<const char*, int> table;
};
