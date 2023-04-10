#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Person
{
  public:
    Person(const int id, const string name) : id(id), name(name)
    {}

    int id;
    string name;

    friend ostream& operator<<(ostream& os, const Person& p)
    {
      return os << p.id << " " << p.name << ";";
    }
};

class PersonFactory
{
  public:
    Person create_person(const string& name)
    {
      return Person(num_people_++, name);
    }

  private:
    int num_people_{0};
};

int main()
{
  PersonFactory fac;
  const Person p1 = fac.create_person("Bob");
  const Person p2 = fac.create_person("Jane");
  const Person p3 = fac.create_person("Kevin");

  std::ostringstream oss;
  cout << "p1: " << p1 << endl;
  cout << "p2: " << p2 << endl;
  cout << "p3: " << p3 << endl;

  return 0;
}
