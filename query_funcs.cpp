#include "query_funcs.h"
#include <iomanip>
#include <iostream>
#include <pqxx/pqxx>
#include <sstream>
#include <string>


void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    work W(*C);
    std::stringstream sql;
    sql << "INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, "
         "RPG, APG, SPG, BPG) VALUES ("
      << team_id << ", " << jersey_num << ", " << W.quote(first_name) << ", "
      << W.quote(last_name) << ", " << mpg << ", " << ppg << ", " << rpg << ", " << apg
      << ", " << spg << ", " << bpg << ");";
    W.exec(sql.str());
    W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
      work W(*C);
  stringstream sql;
  sql << "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES ("
      << W.quote(name) << ", " << state_id << ", " << color_id << ", " << wins << ", "
      << losses << ");";
  W.exec(sql.str());
  W.commit();
}


void add_state(connection *C, string name)
{
    work W(*C);
  stringstream sql; 
  sql << "INSERT INTO STATE (NAME) VALUES (" << W.quote(name) << ");";
  W.exec(sql.str());
  W.commit();
}


void add_color(connection *C, string name)
{
      work W(*C);
  stringstream sql; 
  sql << "INSERT INTO COLOR (NAME) VALUES (" << W.quote(name) << ");";
  W.exec(sql.str());
  W.commit();
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
  work W(*C);
  std::stringstream sql;
  if(use_mpg){
    if(sql.str().empty()){
      sql << "SELECT * FROM PLAYER WHERE MPG" <<">=" <<min_mpg <<"AND MPG"<<"<="<<max_mpg;
    } else{
      sql<< " AND MPG >= "<<min_mpg<<" AND MPG <= "<<max_mpg;
    }
  }
  if(use_ppg){
    if(sql.str().empty()){
      sql << "SELECT * FROM PLAYER WHERE PPG" <<">=" <<min_ppg <<"AND PPG"<<"<="<<max_ppg;
    } else{
      sql<< " AND PPG >= "<<min_ppg<<" AND PPG <= "<<max_ppg;
    }
  }
  if(use_rpg){
    if(sql.str().empty()){
      sql << "SELECT * FROM PLAYER WHERE RPG" <<">=" <<min_rpg <<"AND RPG"<<"<="<<max_rpg;
    } else{
      sql<< " AND RPG >= "<<min_rpg<<" AND RPG <= "<<max_rpg;
    }
  }
  if(use_apg){
    if(sql.str().empty()){
      sql << "SELECT * FROM PLAYER WHERE APG" <<">=" <<min_apg <<"AND APG"<<"<="<<max_apg;
    } else{
      sql<< " AND APG >= "<<min_apg<<" AND APG <= "<<max_apg;
    }
  }
  if(use_spg){
    if(sql.str().empty()){
      sql << "SELECT * FROM PLAYER WHERE SPG" <<">=" <<min_spg <<"AND SPG"<<"<="<<max_spg;
    } else{
      sql<< " AND SPG >= "<<min_spg<<" AND SPG <= "<<max_spg;
    }
  }
  if(use_bpg){
    if(sql.str().empty()){
      sql << "SELECT * FROM PLAYER WHERE BPG" <<">=" <<min_bpg <<"AND BPG"<<"<="<<max_bpg;
    } else{
      sql<< " AND BPG >= "<<min_bpg<<" AND BPG <= "<<max_bpg;
    }
  }
  if(sql.str().empty()){
    sql<< "SELECT * FROM PLAYER";
  }
  sql<<";";
  W.exec(sql.str());
  W.commit();
  nontransaction N(*C);
  result R( N.exec( sql.str() ));
  std::cout <<"PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"<<std::endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c){
    std::cout << c[0].as<string>() << " "<<c[1].as<int>()<< " "<<c[2].as<int>()<< " "<<c[3].as<string>()
    << " "<<c[4].as<string>()<< " "<<c[5].as<int>()<< " "<<c[6].as<int>()<< " "<<c[7].as<int>()
    << " "<<c[8].as<int>()<< " " << fixed << std::setprecision(1) << " "<<c[9].as<double>()<< " "<<c[10].as<double>()<<std::endl;
  }

}

// std::stringstream checkQuery1(int use, int min, int max,std::string pg, std::stringstream sql){
//   if(use){
//     if(sql.str().empty()){
//       sql << "SELECT * FROM PLAYER WHERE " << pg <<">=" <<min <<"AND"<<pg <<"<="<<max<<";";
//     } else{
//       sql<< " AND "<<pg <<">=" <<min <<"AND"<<pg <<"<="<<max<<";";
//     }
//   }
//   return sql;
// }



void query2(connection *C, string team_color)
{
    work W(*C);
    std::stringstream sql;
    sql << "SELECT TEAM.NAME FROM TEAM, COLOR WHERE COLOR.COLOR_ID = TEAM.COLOR_ID AND COLOR.NAME = " << W.quote(team_color) << ";";
    W.exec(sql.str());
    W.commit();
    nontransaction N(*C);
    result R( N.exec( sql.str() ));
    std::cout << "NAME"<<std::endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c){
      std::cout << c[0].as<string>() << std::endl;
    }
}


void query3(connection *C, string team_name)
{
    work W(*C);
    std::stringstream sql;
    sql << "SELECT FIRST_NAME, LAST_NAME FROM PLAYER,TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.NAME = " << W.quote(team_name) << "ORDER BY PLAYER.PPG DESC;";
    W.exec(sql.str());
    W.commit();
    nontransaction N(*C);
    result R( N.exec( sql.str() ));
    std::cout << "FIRST_NAME LAST_NAME"<<std::endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c){
      std::cout << c[0].as<string>() << " "<<c[1].as<string>()<<std::endl;
    }
}


void query4(connection *C, string team_state, string team_color)
{
    work W(*C);
    std::stringstream sql;
    sql << "SELECT FIRST_NAME, LAST_NAME,UNIFORM_NUM FROM PLAYER,TEAM,STATE,COLOR WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.STATE_ID = STATE.STATE_ID AND TEAM.COLOR_ID = COLOR.COLOR_ID AND COLOR.NAME = " 
    << W.quote(team_color) << "AND STATE.NAME = "<<W.quote(team_state) <<";";
    W.exec(sql.str());
    W.commit();
    nontransaction N(*C);
    result R( N.exec( sql.str() ));
    std::cout << "FIRST_NAME LAST_NAME"<<std::endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c){
      std::cout << c[0].as<string>() << " "<<c[1].as<string>()<< " "<<c[2].as<int>()<<std::endl;
    }
}


void query5(connection *C, int num_wins)
{
   
    std::stringstream sql;
    sql << "SELECT FIRST_NAME, LAST_NAME,TEAM.NAME,TEAM.WINS FROM PLAYER,TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.WINS > " 
    <<num_wins<<";";
    work W(*C);
    W.exec(sql.str());
    W.commit();
    nontransaction N(*C);
    result R( N.exec( sql.str() ));
    std::cout << "FIRST_NAME LAST_NAME NAME WINS"<<std::endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c){
      cout << c[0].as<string>() << " " << c[1].as<string>() << " " << c[2].as<string>() << " " << c[3].as<int>() << " " << endl;
    }
}
