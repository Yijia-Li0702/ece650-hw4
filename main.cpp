#include <iostream>
#include <pqxx/pqxx>

#include "exerciser.h"
#include <fstream>
#include <sstream>
using namespace std;
using namespace pqxx;



void createPlayer(connection * C){
      std::string sql;
        /* Create SQL statement */
      sql = "CREATE TABLE PLAYER("  \
      "PLAYER_ID SERIAL NOT NULL," \
      "TEAM_ID INT NOT NULL," \
      "UNIFORM_NUM INT NOT NULL," \
      "FIRST_NAME VARCHAR(256)  NOT NULL," \
      "LAST_NAME  VARCHAR(256) NOT NULL,"\
     "MPG  INT NOT NULL,"\
     "PPG INT NOT NULL,"\
     "RPG INT NOT NULL,"\
     "APG INT NOT NULL,"\
     "SPG DOUBLE PRECISION NOT NULL,"\
     "BPG DOUBLE PRECISION NOT NULL,"\
     "PRIMARY KEY (PLAYER_ID),"\
     "FOREIGN KEY (TEAM_ID) REFERENCES TEAM(TEAM_ID) ON DELETE SET NULL ON UPDATE CASCADE);";

      /* Create a transactional object. */
      work W(* C);

      /* Execute SQL query */
      W.exec( sql );
      W.commit();
      std::cout << "Table created successfully" << std::endl;
}

void createTEAM(connection * C){
  std::string sql;
   sql = "CREATE TABLE TEAM("  \
   "TEAM_ID SERIAL NOT NULL,"\
   "NAME VARCHAR(256)  NOT NULL,"\
   "STATE_ID INT  NOT NULL,"\
   "COLOR_ID INT  NOT NULL,"\
   "WINS INT  NOT NULL,"\
   "LOSSES INT  NOT NULL,"\
    "PRIMARY KEY (TEAM_ID),"\
    "FOREIGN KEY (STATE_ID) REFERENCES STATE(STATE_ID) ON DELETE SET NULL ON UPDATE CASCADE,"\
    "FOREIGN KEY (COLOR_ID) REFERENCES COLOR(COLOR_ID) ON DELETE SET NULL ON UPDATE CASCADE);";
      work W(* C);
      W.exec( sql );
      W.commit();
}
void createSTATE(connection * C){
  std::string sql;
  sql = "CREATE TABLE STATE("  \
  "STATE_ID SERIAL NOT NULL,"\
   "NAME VARCHAR(256)  NOT NULL,"\
   "PRIMARY KEY (STATE_ID));";
    work W(* C);
    W.exec( sql );
    W.commit();
}

void creatCOLOR(connection * C){
  std::string sql;
  sql = "CREATE TABLE COLOR("  \
  "COLOR_ID SERIAL NOT NULL,"\
   "NAME VARCHAR(256)  NOT NULL,"\
   "PRIMARY KEY (COLOR_ID));";
    work W(* C);
    W.exec( sql );
    W.commit();
}

  void dropTables(connection * C){
      std::string sql = "DROP TABLE IF EXISTS PLAYER CASCADE;"\
      "DROP TABLE IF EXISTS TEAM CASCADE;"\
      "DROP TABLE IF EXISTS STATE CASCADE;"\
      "DROP TABLE IF EXISTS COLOR CASCADE;";
      work W(* C);
      W.exec( sql );
      W.commit();
  }

void addPlayer(connection * C){
  std::string playerID,firstName, lastName,line;
  int teamID, uniformNum,mpg,ppg,rpg,apg;
  double spg,bpg;
  std::ifstream ifs;
  ifs.open("player.txt",std::ifstream::in);
  while(getline(ifs,line)){
    std::stringstream str;    
    str<<line;
    str>>playerID>>teamID>>uniformNum>>firstName>>lastName>>mpg>>ppg>>rpg>>apg>>spg>>bpg;
    add_player(C,teamID,uniformNum,firstName,lastName,mpg,ppg,rpg,apg,spg,bpg);
  }
  ifs.close();
}
void addTeam(connection * C){
  std::string teamID,Name,line;
  int stateID, colorID,wins,losses;
  std::ifstream ifs;
  ifs.open("team.txt",std::ifstream::in);
  while(getline(ifs,line)){
    //good use of stringstream
    std::stringstream str;    
    str<<line;
    str>>teamID>>Name>>stateID>>colorID>>wins>>losses;
    add_team(C,Name,stateID,colorID,wins,losses);
  }
  ifs.close();
}

void addState(connection * C){
  std::string stateID,Name,line;
  std::ifstream ifs;
  ifs.open("state.txt",std::ifstream::in);
  while(getline(ifs,line)){
    //good use of stringstream
    std::stringstream str;    
    str<<line;
    str>>stateID>>Name;
    add_state(C,Name);
  }
  ifs.close();
}

void addColor(connection * C){
  std::string colorID,Name,line;
  std::ifstream ifs;
  ifs.open("color.txt",std::ifstream::in);
  while(getline(ifs,line)){
    //good use of stringstream
    std::stringstream str;    
    str<<line;
    str>>colorID>>Name;
    add_color(C,Name);
  }
  ifs.close();
}


int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      std::cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }

  dropTables(C);
  creatCOLOR(C);
  createSTATE(C);
  createTEAM(C);
  createPlayer(C);
  addColor(C);
  addState(C);
  addTeam(C);
  addPlayer(C);
  


  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  exercise(C);


  //Close database connection
  C->disconnect();
  delete C;
  return 0;
}