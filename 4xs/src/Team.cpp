#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "Team.h"
#include "Teacher.h"
#include "Student.h"

#define TEAMS_FILE "teams.csv"
#define DELIMITER ','

Team::Team() {}
Team::Team(int id) : id(id) { }

int Team::getId() {
  return this->id;
}

string Team::getName() {
  return this->name;
}

string Team::getDescription() {
  return this->description;
}

int Team::getTeacherId() {
  return this->teacherId;
}

vector<int> Team::getStudentIds() {
  return this->studentIds;
}

vector<Student> Team::getStudents() {
  vector<Student> students;
  for (int id : this->studentIds) {
    Student* student = new Student(id);
    student->read();
    students.push_back(*student);
    delete(student);
  }

  return students;
}

void Team::setId(int id) {
  this->id = id;
}

void Team::setName(string name) {
  this->name = name;
}

void Team::setDescription(string description) {
  this->description = description;
}

void Team::setTeacherId(int teacherId) {
  this->teacherId = teacherId;
}

void Team::setTeacher(Teacher* tc) {
  this->teacherId = tc->getId();
}

void Team::setStudents(vector<Student*> students) {
  this->studentIds.clear();
  for (Student* student : students) {
    this->studentIds.push_back(student->getId());
  }
}

void Team::setStudentIds(vector<int> studentIds) {
  this->studentIds = studentIds;
}

string serializeStudentsToString(const vector<int> &students) {
  vector<int>::const_iterator it;
  stringstream ss;
  for (it = students.begin(); it != students.end(); ++it) {
    if (it != students.begin()) {
      ss << "|";
    }
    ss << *it;
  }

  return ss.str().c_str();
}

vector<int> parseStudentsFromString(const string &students) {
  vector<int> ids;
  stringstream ss(students);
  string id;

  while (getline(ss, id, '|')) {
    ids.push_back(stoi(id));
  }

  return ids;
}

vector<Team> Team::readAll() {
  ifstream file;
  file.open(TEAMS_FILE);

  string line;
  vector<string> lines;
  vector<Team> teams;

  while (getline(file, line)) {
    lines.push_back(line);
  }

  for (string l : lines) {
    string field;
    stringstream lineStream(l);
    vector<string> fields;

    while (getline(lineStream, field, DELIMITER)) {
      fields.push_back(field);
    }

    Team team;
    team.setId(stoi(fields[0]));
    team.setName(fields[1]);
    team.setDescription(fields[2]);
    team.setTeacherId(stoi(fields[3]));
    team.setStudentIds(parseStudentsFromString(fields[4]));
    
    teams.push_back(team);
  }

  file.close();

  return teams;
}

void Team::create() {
  ofstream file;
  file.open(TEAMS_FILE, ios::out | ios::app);

  file
    << to_string(id) << DELIMITER
    << name << DELIMITER
    << description << DELIMITER
    << to_string(teacherId) << DELIMITER
    << "-1" << endl;

  file.close();
}

Team Team::read() {
  vector<Team> teams = Team::readAll();

  for (Team t : teams) {
    if (this->id == t.getId()) {
      this->id = t.getId();
      this->name = t.getName();
      this->description = t.getDescription();
      this->teacherId = t.getTeacherId();
      this->studentIds = t.getStudentIds();// go fetch from line
    }
  }

  if (this->name.empty()) {
    this->id = -1;
  }

  return *this;
}

Team Team::update() {
  vector<Team> teams = Team::readAll();

  ofstream file;
  file.open(TEAMS_FILE, ios::out | ios::trunc);

  for (Team t : teams) {
    if (t.getId() == this->id) {
      t.setName(this->name);
      t.setDescription(this->description);
      t.setTeacherId(this->teacherId);
      t.setStudentIds(this->studentIds);
    }

    file
      << to_string(t.getId()) << DELIMITER
      << t.getName() << DELIMITER
      << t.getDescription() << DELIMITER
      << to_string(t.getTeacherId()) << DELIMITER
      << serializeStudentsToString(t.studentIds) << endl;
  }

  file.close();

  return *this;
}

void Team::destroy() {
  vector<Team> teams = Team::readAll();

  ofstream file;
  file.open(TEAMS_FILE);

  for (Team t : teams) {
    if (t.getId() != this->id) {
      file
        << to_string(t.getId()) << DELIMITER
        << t.getName() << DELIMITER
        << t.getDescription() << DELIMITER
        << to_string(t.getTeacherId()) << DELIMITER
        << serializeStudentsToString(t.getStudentIds());
    }
  }

  file.close();
}
