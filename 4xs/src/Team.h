#include <string>
#include <vector>

using namespace std;

class Teacher;
class Student;
class Team {

private:
  
  int id;
  string name;
  string description;
  int teacherId;
  vector<int> studentIds;
  string project;

public:

  Team();
  Team(int id);
  Team(string name);

  // Getters
  int getId();
  string getName();
  string getDescription();
  int getTeacherId();
  vector<int> getStudentIds();
  string getStudentIdsAsString();
  vector<Student> getStudents();
  string getProject();
  
  // Setters
  void setId(int id);
  void setName(string name);
  void setDescription(string description);
  void setTeacherId(int teacherId);
  void setTeacher(Teacher* teacher);
  void setStudents(vector<Student*> students);
  void setStudentIds(vector<int> studentIds);
  void setProject(string project);

  // CRUD
  static vector<Team> readAll();
  void create();
  Team read();
  Team update();
  void destroy();
};
