#include <map>

class Symtable {
public:
	static Symtable* getInstance();
	void addVar(char* name, int value);
	int getVar(char* name);
	void setVar(char* name, int newValue);
	bool hasVar(char* name);
private:
	Symtable();

	static Symtable* instance;
	std::map<char*, int> table;
};