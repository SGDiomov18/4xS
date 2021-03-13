#include <string>
#include <vector>

using namespace std;

class Team;
class School;
class Student {

private:

  int id;
  string name;
  string surname;
  string email;
  string role;
  int teamId;
  int schoolId;
  
public:

  Student();
  Student(int id);

  // Getters
  int getId();
  string getName();
  string getSurname();
  string getEmail();
  string getRole();
  int getTeamId();
  int getSchoolId();
  Team* getTeam();
  School* getSchool();

  // Setters
  void setId(int id);
  void setName(string name);
  void setSurname(string surname);
  void setEmail(string email);
  void setRole(string role);
  void setTeamId(int teamId);
  void setSchoolId(int schoolId);

  // CRUD
  static vector<Student> readAll();
  void create();
  Student read();
  Student update();
  void destroy();
};
