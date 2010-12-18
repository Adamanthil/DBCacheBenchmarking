#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <new>

#include "Parser.h"
#include "Settings.h"

#include "Database.h"
#include "FileManager.h"
#include "BufferManager.h"
#include "Query.h"

#include "Schema.h"
#include "Attribute.h"
#include "Operators.h"

#include "Operand.h"
#include "BooleanExpression.h"
#include "DataCreator.h"

using namespace lexer;

typedef ConstantOperand<int> IntConstant;
typedef ConstantOperand<const char *> StringConstant;

typedef VariableOperand<int> IntVariable;
typedef VariableOperand<const char *> StringVariable;

void query(const std::string & query)
{
  Parser p;
  IRelationalOperator * op = p.parse(query);

  op->dump(std::cout);
  delete op;
}

void populate()
{
  
}

void usage()
{
  
}

void imode()
{
  std::string cmd;
  std::string history;

  Database * db = Database::getInstance();

  std::cout << "> ";
  std::cout.flush();

  std::cin >> cmd;
  while (cmd != "quit" && cmd != "exit")
    {
      if (cmd == "tables")
	{
	  const std::vector<Table *> & tables = db->tables();
	  for (int i = 0; i < tables.size(); i++)
	    {
	      std::cout << tables.at(i)->name() << std::endl;
	    }
	}
      else if (cmd == "create")
	{
	  std::string config;
	  std::cin >> config;	 
	}
      else if (cmd == "describe")
	{
	  std::string tbl;
	  std::cin >> tbl; 

	  if (tbl != "" && db->table(tbl))
	    {
	      const Table * table = db->table(tbl);
	      const Schema * schema = table->schema();
	      
	      std::cout << "column\ttype\t\tsize"
			<< std::endl
			<< "======\t====\t\t===="
			<< std::endl;

	      for (int i = 0; i < schema->nitems(); i++)
		{
		  const Attribute * attribute = schema->at(i);
		  std::cout << attribute->name() << "\t" << Attribute::description(attribute->type()) 
			    << "\t\t" << attribute->size() << std::endl;
		}
	    }
	  else
	    {
	      std::cout << "invalid table name. type 'tables' for list of tables" << std::endl;
	    }
	}
      else if (cmd == "query")
	{
	  std::string q;
	  getline(std::cin, q);
	  query(q);
	}
      else if (cmd == "layout")
	{
	  bool enabled = false;
	  Settings::get("partition-materilization", enabled);
	  Settings::set("partition-materilization", !enabled);
	  Settings::get("partition-materilization", enabled);
	  std::cout << "layout set to =" << (enabled ? "partitoned" : "flat")
		    << std::endl;
	}
      else if (cmd == "help")
	{
	  std::cout << "display usage" << std::endl;
	}
      else
	{
	  std::cout << "unknown command" << std::endl
		    << "type help for usage" << std::endl;
	}

      std::cout << "> ";
      std::cout.flush();

      history = cmd;
      std::cin >> cmd;
    }
}

void initialize(const char * catalog = "db.xml", const char * files = "config")
{
  std::cout << "initializing..."; std::cout.flush();
  BufferManager::Initialize();
  std::cout << "loading database info..."; std::cout.flush();
  Database::Initialize(catalog);
  std::cout << "loading database tables..."; std::cout.flush();
  FileManager::Initialize(files);
  std::cout << "done" << std::endl;
};

int main(int argc, char ** argv)
{
  const char * catalog = "db.xml";
  const char * files = "config";

  Settings::set("partition-materilization", true);

  if (argc >= 2)
    catalog = argv[1];
  if (argc >= 3)
    files = argv[2];
  
  initialize(catalog, files);
  imode();
}

#endif
