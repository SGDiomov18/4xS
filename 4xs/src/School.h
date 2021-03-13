#include <string>
#include <vector>

using namespace std;

class Student;
class Teacher;
class School {

private:

  int id;
  string name;
  string city;
  string address;
  vector<int> teacherIds;
  vector<int> studentIds;

public:

  School();
  School(int id);
  
  // Getters
  int getId();
  string getName();
  string getCity();
  string getAddress();
  vector<int> getTeacherIds();
  vector<Teacher> getTeachers();
  vector<int> getStudentIds();
  vector<Student> getStudents();

  // Setters
  void setId(int id);
  void setName(string name);
  void setCity(string city);
  void setAddress(string address);
  void setTeacherIds(vector<int> teacherIds);
  void setTeachers(vector<Teacher*>teachers);
  void setStudentIds(vector<int> studentIds);
  void setStudents(vector<Student*>student);

  // CRUD
  static vector<School> readAll();
  void create();
  School read();
  School update();
  void destroy();
};
