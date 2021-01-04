#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

#define MAX_ID 1024

typedef struct TAG_SYMBOL_ENTRY
{
	string name;
	int token;
	int type;
	bool Isconst=false;
} symbol_entry;

class symbol_table
{
public:
	symbol_entry table[MAX_ID];
	int size;

	int lookup(string name);
	int insert(string name, int token);
	int gettoken(string name);
	string &getname(int pos);
	int set_type(int pos, int type);
	int get_type(int pos);
};
#endif