#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "School.h"
#include "Teacher.h"
#include "Student.h"

#define SCHOOLS_FILE "schools.csv"
#define DELIMITER ','

School::School() {}
School::School(int id) : id(id) {}

int School::getId() {
  return this->id;
}

string School::getName() {
  return this->name;
}

string School::getCity() {
  return this->city;
}

string School::getAddress() {
  return this->address;
}

vector<int> School::getTeacherIds() {
  return this->teacherIds;
}

vector<Teacher> School::getTeachers() {
  vector<Teacher> teachers;
  for (int id : this->teacherIds) {
    Teacher* teacher = new Teacher(id);
    teacher->read();
    teachers.push_back(*teacher);
    delete(teacher);
  }

  return teachers;
}

vector<int> School::getStudentIds() {
  return this->studentIds;
}

vector<Student> School::getStudents() {
  vector<Student> students;
  for (int id : this->studentIds) {
    Student* student = new Student(id);
    student->read();
    students.push_back(*student);
    delete(student);
  }

  return students;
}

string serializeItemsToString(vector<int> &items) {
  vector<int>::const_iterator it;
  stringstream ss;
  for (it = items.begin(); it != items.end(); ++it) {
    if (it != items.begin()) {
      ss << "|";
    }
    ss << *it;
  }

  return ss.str().c_str();
}

vector<int> parseItemsFromString(const string &items) {
  vector<int> ids;
  stringstream ss(items);
  string id;

  while (getline(ss, id, '|')) {
    ids.push_back(stoi(id));
  }

  return ids;
}

void School::setId(int id) {
  this->id = id;
}

void School::setName(string name) {
  this->name = name;
}

void School::setCity(string city) {
  this->city = city;
}

void School::setAddress(string address) {
  this->address = address;
}

void School::setTeacherIds(vector<int> teacherIds) {
  this->teacherIds = teacherIds;
}

void School::setTeachers(vector<Teacher*> teachers) {
  this->teacherIds.clear();
  for (Teacher* teacher : teachers) {
    this->teacherIds.push_back(teacher->getId());
  }
}

void School::setStudentIds(vector<int> studentIds) {
  this->studentIds = studentIds;
}

void School::setStudents(vector<Student*> students) {
  this->studentIds.clear();
  for (Student* student : students) {
    this->studentIds.push_back(student->getId());
  }
}

vector<School> School::readAll() {
  ifstream file;
  file.open(SCHOOLS_FILE);

  string line;
  vector<string> lines;
  vector<School> schools;

  while( getline(file, line)) {
    lines.push_back(line);
  }

  for (string l : lines) {
    string field;
    stringstream lineStream(l);
    vector<string> fields;

    while (getline(lineStream, field, DELIMITER)) {
      fields.push_back(field);
    }

    School school;
    school.setId(stoi(fields[0]));
    school.setName(fields[1]);
    school.setCity(fields[2]);
    school.setAddress(fields[3]);
    school.setTeacherIds(parseItemsFromString(fields[4]));
    school.setStudentIds(parseItemsFromString(fields[5]));
    schools.push_back(school);
  }

  file.close();

  return schools;
}

void School::create() {
  ofstream file;
  file.open(SCHOOLS_FILE, ios::out | ios::app);

  file
    << to_string(id) << DELIMITER
    << name << DELIMITER
    << city << DELIMITER
    << address << DELIMITER
    << "-1" << DELIMITER
    << "-1" << endl;

  file.close();
}

School School::read() {
  vector<School> schools = School::readAll();

  for (School s : schools) {
    if (s.getId() == this->id) {
      this->id = s.getId();
      this->name = s.getName();
      this->city = s.getCity();
      this->address = s.getAddress();
      this->teacherIds = s.getTeacherIds();
      this->studentIds = s.getStudentIds();
    }
  }

  if (this->name.empty()) {
    this->id = -1;
  }

  return *this;
}

School School::update() {
  vector<School> schools = School::readAll();

  ofstream file;
  file.open(SCHOOLS_FILE, ios::out | ios::trunc);

  for (School s : schools) {
    if (s.getId() == this->id) {
      s.setName(this->name);
      s.setCity(this->city);
      s.setAddress(this->address);
      s.setTeacherIds(this->teacherIds);
      s.setStudentIds(this->studentIds);
    }

    file
      << to_string(s.getId()) << DELIMITER
      << s.getName() << DELIMITER
      << s.getCity() << DELIMITER
      << s.getAddress() << DELIMITER 
      << serializeItemsToString(s.teacherIds) << DELIMITER
      << serializeItemsToString(s.studentIds) << endl;
  }

  file.close();

  return *this;
}

void School::destroy() {
  vector<School> schools = School::readAll();

  ofstream file;
  file.open(SCHOOLS_FILE);

  for (School s : schools) {
    if (s.getId() != this->id) {
      file
        << to_string(s.getId()) << DELIMITER
        << s.getName() << DELIMITER
        << s.getCity() << DELIMITER
        << s.getAddress() << DELIMITER
        << serializeItemsToString(s.teacherIds) << DELIMITER
        << serializeItemsToString(s.studentIds) << endl;
    }
  }

  file.close();
}
