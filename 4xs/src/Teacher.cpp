#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "Teacher.h"
#include "School.h"
#include "Team.h"

#define TEACHERS_FILE "teachers.csv"
#define DELIMITER ','

Teacher::Teacher() {}
Teacher::Teacher(int id) : id(id) {}

int Teacher::getId() {
  return this->id;
}

string Teacher::getName() {
  return this->name;
}

string Teacher::getSurname() {
  return this->surname;
}

string Teacher::getEmail() {
  return this->email;
}

int Teacher::getSchoolId() {
  return this->schoolId;
}

School Teacher::getSchool() {
  if (this->schoolId < 0) { return NULL; }

  School school(this->schoolId);
  school.read();
}

vector<int> Teacher::getTeamIds() {
  return this->teamIds;
}

vector<Team> Teacher::getTeams() {
  vector<Team> teams;
  for (int id : this->teamIds) {
    Team* team = new Team(id);
    team->read();
    teams.push_back(*team);
    delete(team);
  }

  return teams;
}

void Teacher::setId(int id) {
  this->id = id;
}

void Teacher::setName(string name) {
  this->name = name;
}

void Teacher::setSurname(string surname) {
  this->surname = surname;
}

void Teacher::setEmail(string email) {
  this->email = email;
}

void Teacher::setSchoolId(int schoolId) {
  this->schoolId = schoolId;
}

void Teacher::setSchool(School* school) {
  this->schoolId = school->getId();
}

void Teacher::setTeams(vector<Team*> teams) {
  this->teamIds.clear();
  for (Team* team : teams) {
    this->teamIds.push_back(team->getId());
  }
}

string serializeToString(const vector<int> &object) {
  vector<int>::const_iterator it;
  stringstream ss;
  for (it = object.begin(); it != object.end(); ++it) {
    if (it != object.begin()) {
      ss << "|";
    }
    ss << *it;
  }

  return ss.str().c_str();
}

vector<int> parseFromString(const string &object) {
  vector<int> ids;
  stringstream ss(object);
  string id;

  while (getline(ss, id, '|')) {
    ids.push_back(stoi(id));
  }

  return ids;
}

void Teacher::setTeamIds(vector<int> teamIds) {
  this->teamIds = teamIds;
}

vector<Teacher> Teacher::readAll() {
  ifstream file;
  file.open(TEACHERS_FILE);

  string line;
  vector<string> lines;
  vector<Teacher> teachers;

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

    Teacher teacher;
    teacher.setId(stoi(fields[0]));
    teacher.setName(fields[1]);
    teacher.setSurname(fields[2]);
    teacher.setEmail(fields[3]);
    teacher.setSchoolId(stoi(fields[4]));
    teacher.setTeamIds(parseFromString(fields[5]));

    teachers.push_back(teacher);
  }

  file.close();

  return teachers;
}

void Teacher::create() {
  ofstream file;
  file.open(TEACHERS_FILE, ios::out | ios::app);

  file
    << to_string(id) << DELIMITER
    << name << DELIMITER
    << surname << DELIMITER
    << email << DELIMITER
    << to_string(schoolId) << DELIMITER
    << "-1" << endl;

  file.close();
}

Teacher Teacher::read() {
  vector<Teacher> teachers = Teacher::readAll();

  for (Teacher t : teachers) {
    if (t.getId() == this->id) {
      this->id = t.getId();
      this->name = t.getName();
      this->surname = t.getSurname();
      this->email = t.getEmail();
      this->schoolId = t.getSchoolId();
    }
  }

  if (this->name.empty()) {
    this->id = -1;
  }

  return *this;
}

Teacher Teacher::update() {
  vector<Teacher> teachers = Teacher::readAll();

  ofstream file;
  file.open(TEACHERS_FILE, ios::out | ios::trunc);

  for (Teacher t : teachers) {
    if (t.getId() == this->id) {
      t.setName(this->name);
      t.setSurname(this->surname);
      t.setEmail(this->email);
      t.setSchoolId(this->schoolId);
    }

    file
      << to_string(t.getId()) << DELIMITER
      << t.getName() << DELIMITER
      << t.getSurname() << DELIMITER
      << t.getEmail() << DELIMITER
      << to_string(t.getSchoolId()) << DELIMITER
      << serializeToString(t.teamIds) << endl;
  }

  file.close();

  return *this;
}

void Teacher::destroy() {
  vector<Teacher> teachers = Teacher::readAll();

  ofstream file;
  file.open(TEACHERS_FILE);

  for (Teacher t : teachers) {
    if (t.getId() != this->id) {
      file
        << to_string(t.getId()) << DELIMITER
        << t.getName() << DELIMITER
        << t.getSurname() << DELIMITER
        << t.getEmail() << DELIMITER
        << to_string(t.getSchoolId()) << DELIMITER
        << serializeToString(t.teamIds) << endl;
    }
  }

  file.close();
}
