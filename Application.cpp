#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <new>

#include "parser.h"

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

void ProjectionFilter(const Table & table)
{
}

void LoopJoin(const Table & table1, const Table & table2) 
{
}

void CartesianJoin(const Table & table1, const Table & table2) 
{
}

void EquiJoin(const Table & t1, const Table & t2)
{
}

void SelectWhere(const Table & tbl)
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

void initialize(const char * db = "db.xml", const char * files = "config")
{
  std::cout << "initializing..."; std::cout.flush();
  BufferManager::Initialize();
  std::cout << "loading database info..."; std::cout.flush();
  Database::Initialize(db);
  std::cout << "loading relations..."; std::cout.flush();
  FileManager::Initialize(files);
  std::cout << "done" << std::endl;
};

int main(int argc, char ** argv)
{

  initialize();
  imode();

  /*
  Schema schema;
  Schema projection;
  
  std::cerr << "initializing...";
  //DataCreator::CreateDB("createdb", false);
  
  BufferManager::Initialize(4096);
  FileManager::Initialize(argv[1], "db.xml");
  Database::Initialize("db.xml");
  Database * db = Database::getInstance();
  
  std::cerr << "complete" << std::endl;

  const Table * t = db->table("test1");
  const Table * t0 = db->table("test2");
  
  // SelectAll(*t);
  ProjectionFilter(*t);
  //SelectAll(*t0);
  //SelectWhere(*t);

  //CartesianJoin(*t,*t0);
  //EquiJoin(*t, *t0);
  //LoopJoin(*t,*t);
  */
}

#endif
