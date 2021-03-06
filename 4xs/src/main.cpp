#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <windows.h>

#include "Team.h"
#include "Teacher.h"
#include "Student.h"
#include "School.h"

int getHighestId(string name) {
  string filename = name + ".csv";
  ifstream ifile;
  ifile.open(filename);

  if (!ifile) {
    return 1;
  }

  string line;
  vector<string> lines;
  vector<int> allIds;

  while (getline(ifile, line)) {
    lines.push_back(line);
  }

  if (lines.size() == 0) {
    return 1;
  }
 
  for (string l : lines) {
    string field;
    stringstream lineStream(l);
    vector<string> fields;

    while (getline(lineStream, field, ',')) {
      fields.push_back(field);
    }
    allIds.push_back(stoi(fields[0]));
  }

  ifile.close();
  
  sort(allIds.begin(), allIds.end(), greater <>());
  return allIds[0] + 1;
}

string serializeIdsToString(const vector<int> &items) {
  string ids = "";

  for (unsigned int i = 0; i < items.size(); i++) {
    // access memory at indexed location
    string a = to_string(items[i]);
    if (i != items.size() - 1) {
      a += '|';
    }
    ids += a;
  }

  return ids;
}

int readId() {
  int input;
  bool valid = false;

  while (!valid) {
    cin >> input;

    if (cin.fail()) {
      cout << "Wrong input" << endl;
      // clear error flags
      cin.clear();
      cin.ignore(INT_MAX, '\n');
    } else {
        valid = true;
    }
  }
  return input;
}

namespace teamsmenu {

  void readAll() {
    std::vector<Team> teams = Team::readAll();
    
    if (teams.size() < 1) {
      cout << endl << "No teams found." << endl << endl;
    } else {
      cout << "----------------All Teams------------------" << endl;
      for(Team t : teams) {
        cout << "Team Id: " << t.getId() << endl;
        cout << "Team Name :" << t.getName() << endl;
        cout << "Team Description :" << t.getDescription() << endl;
        cout << "Team Student Ids: ";
        for (int id : t.getStudentIds()) {
          cout << id << ",";
        }
        cout << endl;
        cout << "-----------------------------------------" << endl;
      }

    }
  }

  void read() {
    bool showMenu = true;

    int id;
    cout << "Please provide a valid Team ID: " << endl;
    id = readId();
    Team team(id);
    team.read();
    if (team.getId() <= 0) {
      cout << "Could not find team with ID, " << id << endl;
    } else {
      while (showMenu) {
        cout << "----------------Team------------------" << endl;
        cout << "Team Id: " << team.getId() << endl;
        cout << "Team Name: " << team.getName() << endl;
        cout << "Team Description: " << team.getDescription() << endl;
        cout << "Team Project: " << team.getProject() << endl;

        if (team.getTeacherId() < 1) {
          cout << "Team Teacher: [not set]" << endl;

        } else {
          int teacherId = team.getTeacherId();
          cout << "Team Teacher ID: " << teacherId << endl;
          Teacher teacher = Teacher(teacherId);
          teacher.read();
          // cout << "\tTeacher name: " << teacher.getName() << "\n" << "\tTeacher surname: "<< teacher.getSurname() << "\n" << "\tTeacher email: " << teacher.getEmail() << endl;
        }

        if (team.getStudentIds().size() <= 0) {
          cout << "Team Student Ids: [not set]" << endl;
        } else {
          cout << "Team Student Ids: " << serializeIdsToString(team.getStudentIds()) << endl;
        }

        for (int id : team.getStudentIds()) {
          cout << to_string(id) << ",";
        }
        cout << endl;

        cout << endl << "Options:" << endl << "------------" << endl;
        cout << "1. Show Teacher" << endl;
        cout << "2. Show Students" << endl;
        cout << "3. Back" << endl;
        cout << endl;

        int option;
        cout << "Enter a number: ";
        option = readId();

        if (option == 1) {
          int teacherId = team.getTeacherId();
          Teacher teacher(teacherId);
          teacher.read();
          if (teacherId < 1 || teacher.getId() < 1) {
            cout << endl << "No teacher found." << endl << endl;
          } else {
            cout << "----------------Teacher of team " << team.getName() << "------------------" << endl;
            cout << "Teacher ID: " << teacher.getId() << endl;
            cout << "Teacher Name: " << teacher.getName() << endl;
            cout << "Teacher Surname: " << teacher.getSurname() << endl;
            cout << "Teacher Email: " << teacher.getEmail() << endl;
            cout << "Teacher School ID: " << teacher.getSchoolId() << endl;
            cout << "Teacher Team IDS: " << serializeIdsToString(teacher.getTeamIds()) << endl;
          }
        } else if (option == 2) {
          if (team.getStudentIds().size() <= 0) {
              cout << endl << "no students found." << endl << endl;
          } else {
            cout << "-------------------Students for team - " << team.getName() << "-----------------" << endl;
            for (Student student : team.getStudents()) {
              if (student.getId() >= 1) {
                cout << "Student ID: " << student.getId() << endl;
                cout << "Student Name: " << student.getName() << endl;
                cout << "Student Surname: " << student.getSurname() << endl;
                cout << "Student Email: " << student.getEmail() << endl;
                cout << "Student Role: " << student.getRole() << endl;
                cout << "--------------------------------------" << endl;
              }
            }
          }
        } else {
          cout << "Returning to previous menu." << endl;
          showMenu = false;
        }

      }
    }
    cout << endl;
    cout << "-----------------------------------------------" << endl;
  }

  void create() {
    string name, description, project;
    cout << "Enter team name:" << endl;
    cin >> name;

    cout << "Enter team description:" << endl;
    cin >> description;

    cout << "Enter team project:" << endl;
    cin >> project;
    Team newTeam;
    newTeam.setName(name);
    newTeam.setDescription(description);
    newTeam.setProject(project);
    newTeam.setId(0);
    newTeam.setTeacherId(-1);
    newTeam.setId(getHighestId("teams"));
    newTeam.create();

    cout << "Created team with ID " + to_string(newTeam.getId()) + "!" << endl;
  }

  void update() {
    bool showMenu = true;
    int userChoice = 0;
    int id;
    cout << "Please provide a valid Team ID: ";
    id = readId();
    Team team(id);
    team.read();

    if (team.getId() < 1) {
      cout << "Team with this ID doesn't exist." << endl;
    } else {
      while (showMenu) {
        string teamName, description, idInIteration, project;
        int teacherId;
        
        cout << endl;
        cout << "Editing: " << team.getName() << endl << "------------" << endl;
        cout << "Team Id: " << team.getId() << endl;
        cout << "Team Name: " << team.getName() << endl;
        cout << "Team Description: " << team.getDescription() << endl;
        cout << "Team Project: " << team.getProject() << endl;
        cout << "Team TeacherId: " << team.getTeacherId() << endl;
        cout << "Team StudentIds: " << serializeIdsToString(team.getStudentIds()) << endl;
        
        cout << endl << "Options:" << endl << "------------" << endl;
        cout << "1. Edit name" << endl;
        cout << "2. Edit description" << endl;
        cout << "3. Edit project" << endl;
        cout << "4. Edit teacherId" << endl;
        cout << "5. Edit studentIds" << endl;
        cout << "6. Save & Stop editing team" << endl;
        cout << endl;

        cout << "Enter a number: ";
        cin >> userChoice;

        if (userChoice == 1) {

          cout << "Enter team's new name: " << endl;
          cin >> teamName;
          team.setName(teamName);

        } else if (userChoice == 2) {

          cout << "Enter team's description: " << endl;
          cin >> description;
          team.setDescription(description);

        } else if (userChoice == 3) {

          cout << "Enter teams's new project: " << endl;
          cin >> project;
          team.setProject(project);

        } else if (userChoice == 4) {

          cout << "Enter teams's new teacher Id: " << endl;
          cin >> teacherId;
          team.setTeacherId(teacherId);

        } else if (userChoice == 5) {

          vector<int> studentIds;
          string ids;
          string id;

          cout << "Enter team's new comma-separated student Ids: " << endl;
          cin >> ids;

          stringstream lineStream(ids);

          while (getline(lineStream, id, ',')) {
            studentIds.push_back(stoi(id));
          }

          team.setStudentIds(studentIds);

        } else {
          cout << "Returning to previous menu." << endl;
          Sleep(1000);
          showMenu = false;
        }

        team.update();
      }
    }
  }

  void destroy() {
    int id;
    cout << "Please provide a valid ID: " << endl;
    id = readId();
    Team team(id);
    team.read();

    if (team.getId() < 1) {
      cout << "Team with this ID doesn't exist." << endl;
    } else {
      cout << "Destroying team with ID: " + to_string(id) << endl;
      team.destroy();
    }
  }

}

namespace studentsmenu {

  void readAll() {
    vector<Student> students = Student::readAll();
    if (students.size() < 1) {
      cout << endl << "No students found." << endl << endl;
    } else {
      cout << "----------------All Students-----------------" << endl;
      for (Student s : students) {
        cout << "Student ID: " << s.getId() << endl;
        cout << "Student Name: " << s.getName() << endl;
        cout << "Student email: " << s.getEmail() << endl;
        cout << "Student role: " << s.getRole() << endl;

        if (s.getTeamId() == -1) {
          cout << "Student Team ID: " << "[not set]" << endl;
        } else {
          cout << "Student Team ID: " << s.getTeamId() << endl;
        }

        if (s.getSchoolId() == -1) {
          cout << "Student School ID: " << "[not set]" << endl;
        } else {
          cout << "Student School ID: " << s.getSchoolId() << endl;
        }

        cout << "-----------------------------------------------" << endl;
      }
    }
  }

  void create() {
    string name, surname, email, role;
    cout << "Enter student name: " << endl;
    cin >> name;
    cout << "Enter student surname: " << endl;
    cin >> surname;
    cout << "Enter student email: " << endl;
    cin >> email;
    cout << "Enter student role: " << endl;
    cin >> role;
    Student newStudent;
    newStudent.setId(getHighestId("students"));
    newStudent.setName(name);
    newStudent.setSurname(surname);
    newStudent.setEmail(email);
    newStudent.setRole(role);
    newStudent.setTeamId(-1);
    newStudent.setSchoolId(-1);
    newStudent.create();
  }

  void read() {
    bool showMenu = true;

    int id;
    cout << "Please provide a valid ID: ";
    id = readId();
    Student student(id);
    student.read();

    if (student.getId() < 1) {
      cout << "Student with this ID doesn't exist." << endl;
    } else {
      while (showMenu) {
        cout << "----------------Student-----------------" << endl;
        cout << "Student ID: " << student.getId() << endl;
        cout << "Student Name: " << student.getName() << endl;
        cout << "Student email: " << student.getEmail() << endl;
        cout << "Student role: " << student.getRole() << endl;

        if (student.getTeamId() < 1) {
          cout << "Student Team ID: [not set]" << endl;
        } else {
          cout << "Student Team ID: " << student.getTeamId() << endl;
        }

        if (student.getTeamId() < 1) {
          cout << "Student School ID: [not set]" << endl;
        } else {
          cout << "Student School ID: " << student.getSchoolId() << endl;
        }

        cout << endl;

        cout << endl << "Options:" << endl << "------------" << endl;
        cout << "1. Show Team:" << endl;
        cout << "2. Show School:" << endl;
        cout << "3. Back" << endl;
        cout << endl;

        int option;
        cout << "Enter a number: ";
        option = readId();

        if (option == 1) {
          int teamId = student.getTeamId();
          Team team(teamId);
          team.read();

          if (teamId < 1 || team.getId() < 1) {
            cout << endl << "No team found." << endl << endl;
          } else {
            cout << "----------------Team of student " << student.getName() << "------------------" << endl;
            cout << "Team ID: " << team.getId() << endl;
            cout << "Team Name: " << team.getName() << endl;
            cout << "Team Description: " << team.getDescription() << endl;
          }
        } else if (option == 2) {
          int schoolId = student.getSchoolId();
          if (schoolId < 1) {
            cout << endl << "No school found." << endl << endl;
          } else {
            School school(schoolId);
            school.read();

            cout << "----------------School-----------------" << endl;
            cout << "School ID: " << school.getId() << endl;
            cout << "School city: " << school.getName() << endl;
            cout << "School address: " << school.getAddress() << endl;
            cout << "-----------------------------------------------" << endl;
          }
        } else {
          cout << "Returning to previous menu." << endl;
          showMenu = false;
        }
      }
    }
  }

  void update() {
    bool showMenu = true;
    int userChoice = 0;

    int id;
    cout << "Please provide a valid ID: ";
    id = readId();

    Student student(id);
    student.read();

    if (student.getId() < 1) {
      cout << "Student with this ID doesn't exist." << endl;
    } else {
      while (showMenu) {
        string name, surname, email, role;
        int teamId, schoolId;

        cout << endl;
        cout << "Editing: " << student.getName() << endl << "------------" << endl;
        cout << "Student Surname: " << student.getSurname() << endl;
        cout << "Student Email: " << student.getEmail() << endl;
        cout << "Student Role: " << student.getRole() << endl;
        cout << "Student TeamId: " << student.getTeamId() << endl;
        cout << "Student SchoolId: " << student.getSchoolId() << endl;
        
        cout << endl << "Options:" << endl << "------------" << endl;
        cout << "1. Edit Name" << endl;
        cout << "2. Edit Surname" << endl;
        cout << "3. Edit Email" << endl;
        cout << "4. Edit Role" << endl;
        cout << "5. Edit TeamId" << endl;
        cout << "6. Edit SchoolId" << endl;
        cout << "7. Save & Stop editing team" << endl;
        cout << endl;

        cout << "Enter a number: ";

        cin >> userChoice;

        if(userChoice == 1) {
          cout << "Enter student name: " << endl;
          cin >> name;
          student.setName(name);
        } else if (userChoice == 2) {
          cout << "Enter student surname: " << endl;
          cin >> surname;
          student.setSurname(surname);
        } else if (userChoice == 3) {
          cout << "Enter student email: " << endl;
          cin >> email;
          student.setEmail(email);
        } else if (userChoice == 4) {
          cout << "Enter student role: " << endl;
          cin >> role;
          student.setRole(role);
        } else if (userChoice == 5) {
          cout << "Enter student's new team Id: " << endl;
          cin >> teamId;
          student.setTeamId(teamId);
        } else if (userChoice == 6) {
          cout << "Enter student's new team Id: " << endl;
          cin >> schoolId;
          student.setSchoolId(schoolId);
        } else {
          cout << "Returning to previous menu." << endl;
          showMenu = false;
        }

        student.update();
      }
    }
  }

  void destroy() {
    int id;
    cout << "Please provide a valid ID: ";
    id = readId();
    Student student(id);
    student.read();

    if (student.getId() < 1) {
      cout << "Student with this ID doesn't exist." << endl;
    } else {
      cout << "Destroying student with ID: " + to_string(id) << endl;
      student.destroy();
    }
  }

}

namespace schoolsmenu {

  void readAll() {
    vector<School> schools = School::readAll();

    if (schools.size() < 1) {
      cout << endl << "No schools found." << endl << endl;
    } else {
      cout << "-------------------All Schools-----------------" << endl;
      for (School s : schools) {
        cout << "School ID: " << s.getId() << endl;
        cout << "School Name: " << s.getName() << endl;
        cout << "School City: " << s.getCity() << endl;
        cout << "School Address: " << s.getAddress() << endl;

        if (s.getTeacherIds().size() <= 0) {
          cout << "School Teacher Ids: [not set]" << endl;
        } else {
          cout << "School Teacher Ids: " << serializeIdsToString(s.getTeacherIds()) << endl;
        }

        if (s.getStudentIds().size() <= 0) {
          cout << "School Student Ids: [not set]" << endl;
        } else {
          cout << "School Student Ids: " << serializeIdsToString(s.getStudentIds()) << endl;
        }

        cout << "-----------------------------------------------" << endl;
      }
    }
  }

  void create() {
    string name, city, address;
    cout << "Enter school name: " << endl;
    cin >> name;
    cout << "Enter school city: " << endl;
    cin >> city;
    cout << "Enter school address: " << endl;
    cin >> address;
    School newSchool;
    newSchool.setId(getHighestId("schools"));
    newSchool.setName(name);
    newSchool.setCity(city);
    newSchool.setAddress(address);
    newSchool.create();

    cout << "Created school with ID " + to_string(newSchool.getId()) + "!" << endl;
  }

  void read() {
    bool showMenu = true;

    cout << "Please provide a valid School ID: " << endl;
    int id = readId();
    School school(id);
    school.read();

    if (school.getId() <= 0) {
      cout << "School with this ID doesn't exist." << endl;
    } else {
      while (showMenu) {
        cout << "----------------School-----------------" << endl;
        cout << "School ID: " << school.getId() << endl;
        cout << "School city: " << school.getName() << endl;
        cout << "School address: " << school.getAddress() << endl;

        if (school.getTeacherIds().size() <= 0) {
          cout << "School Teacher Ids: [not set]" << endl;
        } else {
          cout << "School Teacher Ids: " << serializeIdsToString(school.getTeacherIds()) << endl;
        }

        if (school.getStudentIds().size() <= 0) {
          cout << "School Student Ids: [not set]" << endl;
        } else {
          cout << "School Student Ids: " << serializeIdsToString(school.getStudentIds()) << endl;
        }

        cout << endl << "Options:" << endl << "------------" << endl;
        cout << "1. Show Teachers" << endl;
        cout << "2. Show Students" << endl;
        cout << "3. Back" << endl;
        cout << endl;

        int option;
        cout << "Enter a number: ";
        option = readId();

        if (option == 1) {
          if (school.getTeacherIds().size() <= 0) {
            cout << endl << "No teachers found." << endl;
          } else {
            cout << "-------------------Teachers for school - " << school.getName() << "-----------------" << endl;
            for (Teacher teacher : school.getTeachers()) {
              if (teacher.getId() >= 1) {
                cout << "Teacher ID: " << teacher.getId() << endl;
                cout << "Teacher Name: " << teacher.getName() << endl;
                cout << "Teacher Surname: " << teacher.getSurname() << endl;
                cout << "Teacher Email: " << teacher.getEmail() << endl;
                cout << "--------------------------------------" << endl;
              }
            }
          }
        } else if (option == 2) {
          if (school.getStudentIds().size() <= 0) {
            cout << endl << "No students found." << endl;
          } else {
            cout << "-------------------Students for school - " << school.getName() << "-----------------" << endl;
            for (Student student : school.getStudents()) {
              if (student.getId() >= 1) {
                cout << "Student ID: " << student.getId() << endl;
                cout << "Student Name: " << student.getName() << endl;
                cout << "Student Surname: " << student.getSurname() << endl;
                cout << "Student Email: " << student.getEmail() << endl;
                cout << "Student Role: " << student.getRole() << endl;
                cout << "--------------------------------------" << endl;
              }
            }
          }
        } else {
          cout << "Returning to previous menu." << endl;
          showMenu = false;
        }
      }
    }
    cout << endl;
    cout << "-----------------------------------------------" << endl;
  }

  void update() {
    bool showMenu = true;
    int userChoice = 0;
    int id;
    cout << "Please provide a valid School ID: ";
    id = readId();
    School school(id);
    school.read();

    if (school.getId() <= 0) {
      cout << "School with this ID doesn't exist." << endl;
      update();
    } else {
      while (showMenu) {
        string name, city, address, idInIteration;

        cout << endl;
        cout << "Editing: " << school.getName() << endl << "------------" << endl;
        cout << "School Id: " << school.getId() << endl;
        cout << "School Name: " << school.getName() << endl;
        cout << "School City: " << school.getCity() << endl;
        cout << "School Address: " << school.getAddress() << endl;
        cout << "School TeacherIds: " << serializeIdsToString(school.getTeacherIds()) << endl;
        cout << "School StudentIds: " << serializeIdsToString(school.getStudentIds()) << endl;
        
        cout << endl << "Options:" << endl << "------------" << endl;
        cout << "1. Edit name" << endl;
        cout << "2. Edit city" << endl;
        cout << "3. Edit address" << endl;
        cout << "4. Edit teacherIds" << endl;
        cout << "5. Edit studentIds" << endl;
        cout << "6. Save & Stop editing team" << endl;
        cout << endl;

        cout << "Enter a number: ";

        cin >> userChoice;

        if (userChoice == 1) {

          cout << "Enter school's new name: " << endl;
          cin >> name;
          school.setName(name);

        } else if (userChoice == 2) {

          cout << "Enter school's city: " << endl;
          cin >> city;
          school.setCity(city);

        } else if (userChoice == 3) {

          cout << "Enter school's address: " << endl;
          cin >> address;
          school.setAddress(address);

        } else if (userChoice == 4) {

          vector<int> teacherIds;
          string ids;
          string id;

          cout << "Enter school's new comma-separated teacher Ids: " << endl;
          cin >> ids;

          stringstream lineStream(ids);

          while (getline(lineStream, id, ',')) {
            teacherIds.push_back(stoi(id));
          }

          school.setTeacherIds(teacherIds);

        } else if (userChoice == 5) {

          vector<int> studentIds;
          string ids;
          string id;

          cout << "Enter school's new comma-separated student Ids: " << endl;
          cin >> ids;

          stringstream lineStream(ids);

          while (getline(lineStream, id, ',')) {
            studentIds.push_back(stoi(id));
          }

          school.setStudentIds(studentIds);

        } else {
          cout << "Returning to previous menu." << endl;
          showMenu = false;
        }
        school.update();
      }
    }
  }

  void destroy() {
    int id;
    cout << "Please provide a valid ID: ";
    id = readId();
    School school(id);
    school.read();

    if (school.getId() <= 0) {
      cout << "School with this ID doesn't exist." << endl;
      destroy();
    } else {
      cout << "Destroying school with ID: " + to_string(id) << endl;
      school.destroy();
    }
  }

}

namespace teachersmenu {

  void readAll() {
    vector<Teacher> teachers = Teacher::readAll();
    cout << "-------------------All Teachers-----------------" << endl;
    for (Teacher t : teachers) {
      cout << "Teacher ID: " << t.getId() << endl;
      cout << "Teacher Name: " << t.getName() << endl;
      cout << "Teacher Surname: " << t.getSurname() << endl;
      cout << "Teacher Email: " << t.getEmail() << endl;

      if (t.getSchoolId() <= 0) {
        cout << "Teacher School Id: [not set]" << endl;
      } else {
        cout << "Teacher School Id:  " << t.getSchoolId() << endl;
      }

      if (t.getTeamIds().size() <= 0) {
        cout << "Teacher Team Id: [not set]" << endl;
      } else {
        cout << "Teacher Team Ids:  " << serializeIdsToString(t.getTeamIds()) << endl;
      }
      cout << "-----------------------------------------------" << endl;
    }
  }

  void create() {
    string name, surname, email;
    cout << "Enter teacher name: " << endl;
    cin >> name;
    cout << "Enter teacher surname: " << endl;
    cin >> surname;
    cout << "Enter teacher email: " << endl;
    cin >> email;
    Teacher newTeacher;
    newTeacher.setId(getHighestId("teachers"));
    newTeacher.setName(name);
    newTeacher.setSurname(surname);
    newTeacher.setEmail(email);
    newTeacher.setSchoolId(-1);
    newTeacher.create();

    cout << "Created school with ID " + to_string(newTeacher.getId()) + "!" << endl;
  }

  void read() {
    int id;
    cout << "Please provide a valid ID: ";
    id = readId();
    Teacher teacher(id);
    teacher.read();

    if (teacher.getId() <= 0) {
      cout << "Teacher with this ID doesn't exist." << endl;
    } else {
      cout << "----------------Teacher-----------------" << endl;
      cout << "Teacher ID: " << teacher.getId() << endl;
      cout << "Teacher Name: " << teacher.getName() << endl;
      cout << "Teacher Surname: " << teacher.getSurname() << endl;
      cout << "Teacher Email: " << teacher.getEmail() << endl;

      if (teacher.getSchoolId() <= 0) {
        cout << "Teacher School Id: [not set]" << endl;
      } else {
        cout << "Teacher School Id:  " << teacher.getSchoolId() << endl;
      }

      if (teacher.getTeamIds().size() <= 0) {
        cout << "Teacher Team Id: [not set]" << endl;
      } else {
        cout << "Teacher Team Ids:  " << serializeIdsToString(teacher.getTeamIds()) << endl;
      }

      cout << "-----------------------------------------------" << endl;
    }
  }

  void update() {
    bool showMenu = true;
    int userChoice = 0;

    int id;
    cout << "Please provide a valid ID: ";
    id = readId();
    Teacher teacher(id);
    teacher.read();

    if (teacher.getId() <= 0) {
      cout << "Teacher with this ID doesn't exist." << endl;
    } else {
      while(showMenu) {
        string name, surname, email;
        int schoolId;

        cout << endl;
        cout << "Editing: " << teacher.getName() << endl << "------------" << endl;
        cout << "Teacher Id: " << teacher.getId() << endl;
        cout << "Teacher Surname: " << teacher.getSurname() << endl;
        cout << "Teacher Email: " << teacher.getEmail() << endl;
        cout << "Teacher School ID: " << teacher.getSchoolId() << endl;
        cout << "Teacher Team IDs: " << serializeIdsToString(teacher.getTeamIds()) << endl;

        cout << endl << "Options:" << endl << "------------" << endl;
        cout << "1. Edit Name" << endl;
        cout << "2. Edit Surname" << endl;
        cout << "3. Edit Email" << endl;
        cout << "4. Edit School ID" << endl;
        cout << "5. Edit Team Ids" << endl;
        cout << "6. Save & Stop editing team" << endl;
        cout << endl;

        cout << "Enter a number: ";

        cin >> userChoice;

        if (userChoice == 1) {
          cout << "Enter teacher name: " << endl;
          cin >> name;
          teacher.setName(name);
        } else if (userChoice == 2) {
          cout << "Enter teacher surname: " << endl;
          cin >> surname;
          teacher.setSurname(surname);
        } else if (userChoice == 3) {
          cout << "Enter teacher email: " << endl;
          cin >> email;
          teacher.setEmail(email);
        } else if (userChoice == 4) {
          cout << "Enter teacher school ID: " << endl;
          cin >> schoolId;
          teacher.setSchoolId(schoolId);
        } else if (userChoice == 5) {
          
          vector<int> teamIds;
          string ids;
          string id;

          cout << "Enter teacher's new comma-separated team Ids: " << endl;
          cin >> ids;

          stringstream lineStream(ids);

          while (getline(lineStream, id, ',')) {
            teamIds.push_back(stoi(id));
          }

          teacher.setTeamIds(teamIds);

        } else {
          cout << "Returning to previous menu." << endl;
          showMenu = false;
        }
        teacher.update();
      }
    }
  }

  void destroy() {
    int id;
    cout << "Please provide a valid ID: ";
    id = readId();
    Teacher teacher(id);
    teacher.read();

    if (teacher.getId() <= 0) {
      cout << "Teacher with this ID doesn't exist." << endl;
    } else {
      cout << "Destroying teacher with ID: " + to_string(id) << endl;
      teacher.destroy();
    }
  }
}

void mainMenu() {
  bool showMainMenu = true;
  while (showMainMenu) {
    cout << "********** MAIN MENU ***********" << endl << endl;
    cout << "1. Schools" << endl;
    cout << "2. Teams" << endl;
    cout << "3. Students" << endl;
    cout << "4. Teachers" << endl;
    cout << "5. Exit" << endl << endl;
    cout << "********************************" << endl << endl;

    int option;
    cout << "Enter a number: ";
    cin >> option;
    cout << endl;

    switch (option) {
    case 1:
      option = 0;

      cout << "********** SCHOOLS MENU ***********" << endl << endl;
      cout << "1. Show all" << endl;
      cout << "2. Show one by ID" << endl;
      cout << "3. Add School" << endl;
      cout << "4. Edit School" << endl;
      cout << "5. Delete School" << endl;
      cout << "6. Back" << endl << endl;
      cout << "*********************************" << endl << endl;
      cout << "Enter a number: ";
      cin >> option;

      while (option < 1 || option > 6) {
        cout << "Enter a valid number between 1 and 6: ";
        cin >> option;
      }
      if (option == 1)
      {
        cout << endl;
        schoolsmenu::readAll();
        cout << endl;
      }
      else if (option == 2)
      {
        cout << endl;
        schoolsmenu::read();
        cout << endl;
      }
      else if (option == 3)
      {
        cout << endl;
        schoolsmenu::create();
        cout << endl;
      }
      else if (option == 4)
      {
        cout << endl;
        schoolsmenu::update();
        cout << endl;
      }
      else if (option == 5)
      {
        cout << endl;
        schoolsmenu::destroy();
        cout << endl;
      }
      else if (option == 6)
      {
        showMainMenu = true;
      }
      break;

    case 2:
      option = 0;

      cout << "********** TEAMS MENU ***********" << endl << endl;
      cout << "1. Show all" << endl;
      cout << "2. Show one by ID" << endl;
      cout << "3. Add team" << endl;
      cout << "4. Edit team" << endl;
      cout << "5. Delete team" << endl;
      cout << "6. Back" << endl << endl;
      cout << "*********************************" << endl << endl;
      cout << "Enter a number: ";
      cin >> option;

      while (option < 1 || option > 6) {
        cout << "Enter a valid number between 1 and 6: ";
        cin >> option;
      }
      if (option == 1)
      {
        cout << endl;
        teamsmenu::readAll();
        cout << endl;
      }
      else if (option == 2)
      {
        cout << endl;
        teamsmenu::read();
        cout << endl;
      }
      else if (option == 3)
      {
        cout << endl;
        teamsmenu::create();
        cout << endl;
      }
      else if (option == 4)
      {
        cout << endl;
        teamsmenu::update();
        cout << endl;
      }
      else if (option == 5)
      {
        cout << endl;
        teamsmenu::destroy();
        cout << endl;
      }
      else if (option == 6)
      {
        showMainMenu = true;
      }
      break;

    case 3:
      option = 0;

      cout << "********** STUDENTS MENU ***********" << endl << endl;
      cout << "1. Show all" << endl;
      cout << "2. Show one by ID" << endl;
      cout << "3. Add student" << endl;
      cout << "4. Edit student" << endl;
      cout << "5. Delete student" << endl;
      cout << "6. Back" << endl << endl;
      cout << "*********************************" << endl << endl;

      cin >> option;

      while (option < 1 || option > 6){
        cout << "Enter a valid number between 1 and 6: ";
        cin >> option;
      }

      if (option == 1)
      {
        cout << endl;
        studentsmenu::readAll();
        cout << endl;
      }
      if (option == 2)
      {
        cout << endl;
        studentsmenu::read();
        cout << endl;
      }
      else if (option == 3)
      {
        cout << endl;
        studentsmenu::create();
        cout << endl;
      }
      else if (option == 4)
      {
        cout << endl;
        studentsmenu::update();
        cout << endl;
      }
      else if (option == 5)
      {
        cout << endl;
        studentsmenu::destroy();
        cout << endl;
      }
      else if (option == 6)
      {
        showMainMenu = true;
      }
      break;

    case 4:
      option = 0;

      cout << "********** TEACHERS MENU ***********" << endl << endl;
      cout << "1. Show all" << endl;
      cout << "2. Show one by ID" << endl;
      cout << "3. Add teacher" << endl;
      cout << "4. Edit teacher" << endl;
      cout << "5. Delete teacher" << endl;
      cout << "6. Back" << endl << endl;
      cout << "*********************************" << endl << endl;

      cin >> option;

      while (option < 1 || option > 6) {
        cout << "Enter a valid number between 1 and 6: ";
        cin >> option;
      }

      if (option == 1) {
        cout << endl;
        teachersmenu::readAll();
        cout << endl;
      }
      else if (option == 2) {
        cout << endl;
        teachersmenu::read();
        cout << endl;
      }
      else if (option == 3) {
        cout << endl;
        teachersmenu::create();
        cout << endl;
      }
      else if (option == 4) {
        cout << endl;
        teachersmenu::update();
        cout << endl;
      }
      else if (option == 5) {
        cout << endl;
        teachersmenu::destroy();
        cout << endl;
      }
      else if (option == 6)
      {
        showMainMenu = true;
      }
      break;

    case 5:
      cout << "Bye :)" << endl << endl;
      exit(0);
      break;
    }
  }
}

void start() {
  try {
    mainMenu();
  } catch (runtime_error& e) {
    cout << "File not found, create some records first :(" << endl << endl;
  }
}

int main() {
  system("Color 0A");
  start();
  return 0;
}