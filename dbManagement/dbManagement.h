#pragma once

//checks for db folder existance and creates if it do not exisit
int db_init();

//adds a file to db
int db_add();

//checks for file existance
int db_check_for_existance();
