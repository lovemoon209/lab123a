#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

enum Speciality { CS, IT, Math, Physics, Engineering };

const char *specialityToStr(Speciality s) {
  switch (s) {
  case CS:
    return "CS";
  case IT:
    return "IT";
  case Math:
    return "Math";
  case Physics:
    return "Physics";
  case Engineering:
    return "Eng";
  default:
    return "Unknown";
  }
}

struct Student {
  int id;
  char surname[32];
  int course;
  Speciality spec;
  int physics;
  int math;
  int informatics;
};

struct Index {
  int pos;
  int course;
  int informatics;
  char surname[32];
};

bool compareStudents(const Student &a, const Student &b) {
  if (a.informatics != b.informatics)
    return a.informatics < b.informatics;
  if (a.course != b.course)
    return a.course < b.course;
  return strcmp(a.surname, b.surname) < 0 ? false : true;
}

bool compareIndex(const Index &a, const Index &b) {
  if (a.informatics != b.informatics)
    return a.informatics < b.informatics;
  if (a.course != b.course)
    return a.course < b.course;
  return strcmp(a.surname, b.surname) < 0;
}

bool binarySearch(Index arr[], int n, const char *surname, int course,
                  int informatics) {
  int left = 0, right = n - 1;
  while (left <= right) {
    int mid = (left + right) / 2;
    int cmp = strcmp(arr[mid].surname, surname);
    if (arr[mid].informatics == informatics && arr[mid].course == course &&
        cmp == 0) {
      return true;
    }
    if (arr[mid].informatics < informatics ||
        (arr[mid].informatics == informatics && arr[mid].course < course) ||
        (arr[mid].informatics == informatics && arr[mid].course == course &&
         cmp < 0)) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return false;
}

void printTable(Student arr[], int n) {
  cout << left << setw(5) << "№" << setw(20) << "Прізвище" << setw(10) << "Курс"
       << setw(15) << "Спец." << setw(15) << "Фізика" << setw(15) << "Матем."
       << setw(10) << "Інформ." << endl;

  cout << string(65, '-') << endl;

  for (int i = 0; i < n; i++) {
    cout << left << setw(5) << arr[i].id << setw(20) << arr[i].surname
         << setw(7) << arr[i].course << setw(12) << specialityToStr(arr[i].spec)
         << setw(8) << arr[i].physics << setw(8) << arr[i].math << setw(10)
         << arr[i].informatics << endl;
  }
}

int main() {
  const int N = 4;
  Student students[N] = {{1, "Іваненко", 2, CS, 85, 90, 95},
                         {2, "Петренко", 1, IT, 70, 80, 75},
                         {3, "Сидоренко", 3, Math, 88, 92, 95},
                         {4, "Коваленко", 2, Physics, 60, 65, 70}};

  ofstream fout("students.ds", ios::binary);
  fout.write((char *)students, sizeof(students));
  fout.close();

  Student loaded[N];
  ifstream fin("students.dats", ios::binary);
  fin.read((char *)loaded, sizeof(loaded));
  fin.close();

  for (int i = 0; i < N - 1; i++)
    for (int j = 0; j < N - i - 1; j++)
      if (compareStudents(loaded[j + 1], loaded[j])) {
        Student tmp = loaded[j];
        loaded[j] = loaded[j + 1];
        loaded[j + 1] = tmp;
      }

  cout << "Таблиця студентів після сортування:\n";
  printTable(loaded, N);

  Index indexFile[N];
  for (int i = 0; i < N; i++) {
    indexFile[i].pos = i;
    indexFile[i].course = loaded[i].course;
    indexFile[i].informatics = loaded[i].informatics;
    strcpy(indexFile[i].surname, loaded[i].surname);
  }

  for (int i = 0; i < N - 1; i++)
    for (int j = 0; j < N - i - 1; j++)
      if (compareIndex(indexFile[j + 1], indexFile[j])) {
        Index tmp = indexFile[j];
        indexFile[j] = indexFile[j + 1];
        indexFile[j + 1] = tmp;
      }

  const char *targetSurname = "Сидоренко";
  int targetCourse = 3;
  int targetInformatics = 95;

  if (binarySearch(indexFile, N, targetSurname, targetCourse,
                   targetInformatics))
    cout << "Студент Сидоренко знайдений!" << endl;
  else
    cout << "Студента не знайдено." << endl;

  return 0;
}
