#include <string>
#include <vector>

using namespace std;

class School;
class Team;
class Teacher {

private:

  int id;
  string name;
  string surname;
  string email;
  int schoolId;
  vector<int> teamIds;

public:

  Teacher();
  Teacher(int id);

  // Getters
  int getId();
  string getName();
  string getSurname();
  string getEmail();
  int getSchoolId();
  School getSchool();
  vector<int> getTeamIds();
  vector<Team> getTeams();

  // Setters
  void setId(int id);
  void setName(string name);
  void setSurname(string surname);
  void setEmail(string email);
  void setSchoolId(int schoolId);
  void setSchool(School* school);
  void setTeamIds(vector<int> teamIds);
  void setTeams(vector<Team*> teams);

  // CRUD
  static vector<Teacher> readAll();
  void create();
  Teacher read();
  Teacher update();
  void destroy();
};
