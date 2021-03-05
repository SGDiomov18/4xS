#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "Student.h"
#include "Team.h"
#include "School.h"

#define STUDENTS_FILE "students.csv"
#define DELIMITER ','

Student::Student() {}
Student::Student(int id): id(id) { }

int Student::getId() {
  return this->id;
}

string Student::getName() {
  return this->name;
}

string Student::getSurname() {
  return this->surname;
}

string Student::getEmail() {
  return this->email;
}

string Student::getRole() {
  return this->role;
}

int Student::getTeamId() {
  return this->teamId;
}

int Student::getSchoolId() {
  return this->schoolId;
}

Team* Student::getTeam() {
  if (this->teamId < 0) { return NULL; }

  Team team(this->teamId);
  team.read();
}

School* Student::getSchool() {
  if (this->schoolId < 0) { return NULL; }

  School school(this->schoolId);
  school.read();
}

void Student::setId(int id) {
  this->id = id;
}

void Student::setName(string name) {
  this->name = name;
}

void Student::setSurname(string surname) {
  this->surname = surname;
}

void Student::setEmail(string email) {
  this->email = email;
}

void Student::setRole(string role) {
  this->role = role;
}

void Student::setTeamId(int teamId) {
  this->teamId = teamId;
}

void Student::setSchoolId(int schoolId) {
  this->schoolId = schoolId;
}

vector<Student> Student::readAll() {
  ifstream file;
  file.open(STUDENTS_FILE);

  string line;
  vector<string> lines;
  vector<Student> students;

  while(getline(file, line)) {
    lines.push_back(line);
  }

  for(string l : lines) {
    string field;
    stringstream lineStream(l);
    vector<string> fields;

    while(getline(lineStream, field, DELIMITER)) {
      fields.push_back(field);
    }

    Student student;
    student.setId(stoi(fields[0]));
    student.setName(fields[1]);
    student.setSurname(fields[2]);
    student.setEmail(fields[3]);
    student.setRole(fields[4]);
    student.setTeamId(stoi(fields[5]));
    student.setSchoolId(stoi(fields[6]));

    students.push_back(student);
  }

  file.close();

  return students;
}

void Student::create() {
  ofstream file;
  file.open(STUDENTS_FILE, ios::out | ios::app);

  file
    << to_string(id) << DELIMITER
    << name << DELIMITER
    << surname << DELIMITER
    << email << DELIMITER
    << role << DELIMITER
    << to_string(teamId) << DELIMITER
    << to_string(schoolId) << endl;

  file.close();
}

Student Student::read() {
  vector<Student> students = Student::readAll();

  for(Student s : students) {
    if(this->id == s.getId()) {
      this->id = s.getId();
      this->name = s.getName();
      this->surname = s.getSurname();
      this->email = s.getEmail();
      this->role = s.getRole();
      this->teamId = s.getTeamId();
      this->schoolId = s.getSchoolId();
    }
  }

  if (this->name.empty()) {
    this->id = -1;
  }

  return *this;
}

Student Student::update() {
  vector<Student> students = Student::readAll();

  ofstream file;
  file.open(STUDENTS_FILE, ios::out | ios::trunc);

  for(Student s : students) {
    if(s.getId() == this->id) {
      s.setName(this->name);
      s.setSurname(this->surname);
      s.setEmail(this->email);
      s.setRole(this->role);
      s.setTeamId(this->teamId);
      s.setSchoolId(this->schoolId);
    }

    file
      << to_string(s.getId()) << DELIMITER
      << s.getName() << DELIMITER
      << s.getSurname() << DELIMITER
      << s.getEmail() << DELIMITER
      << s.getRole() << DELIMITER
      << to_string(s.getTeamId()) << DELIMITER
      << to_string(s.getSchoolId()) << endl;
  }

  file.close();

  return *this;
}

void Student::destroy() {
  vector<Student> students = Student::readAll();

  ofstream file;
  file.open(STUDENTS_FILE);

  for(Student s : students) {
    if(s.getId() != this->id) {
      file
        << to_string(s.getId()) << DELIMITER
        << s.getName() << DELIMITER
        << s.getSurname() << DELIMITER
        << s.getEmail() << DELIMITER
        << s.getRole() << DELIMITER
        << to_string(s.getTeamId()) << DELIMITER
        << to_string(s.getSchoolId()) << endl;
    }
  }

  file.close();
}
