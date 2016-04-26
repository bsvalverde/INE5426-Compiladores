#include <map>

class Symtable {
public:
	static Symtable* getInstance();
	void addVar(std::string name, int value);
	int getVar(std::string name);
	void setVar(std::string name, int newValue);
	bool hasVar(std::string name);
private:
	Symtable();

	static Symtable* instance;
	std::map<std::string, int> table;
};
