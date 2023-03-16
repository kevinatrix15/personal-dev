#include <memory>
#include <ostream>
#include <string>
#include <vector>
using namespace std;

struct Field
{
  Field(const string& name, const string& type) : name(name), type(type)
  {}

  friend ostream& operator<<(ostream& os, const Field& obj)
  {
    return os << obj.type << " " << obj.name << ";";
  }

  string name, type;
};

class CodeBuilder;  // forward decl

class ClassBlock
{
public:
  ClassBlock() = default;

  ClassBlock(const string& name): name_(name) {}

  void addField(const string& name, const string& type)
  {
    fields_.emplace_back(name, type);
  }

  friend ostream& operator<<(ostream& os, const ClassBlock& obj)
  {
    os << "class " << obj.name_ << "\n{\n";
    for (const auto& field : obj.fields_) {
      os << " " << field << "\n";
    }
    return os << "};\n";
  }

  static CodeBuilder build(const string& class_name)
  {
    return {class_name};
  }

private:
  string name_{};
  vector<Field> fields_{};
};

class CodeBuilder
{
public:
  CodeBuilder(const string& class_name)
  {
    root_ = ClassBlock(class_name);
  }

  CodeBuilder& add_field(const string& name, const string& type)
  {
    root_.addField(name, type);
    return *this;
  }

  operator ClassBlock() const
  {
    return root_;
  }

  friend ostream& operator<<(ostream& os, const CodeBuilder& obj)
  {
    return os << obj.root_;
  }

private:
  ClassBlock root_;
};

int main()
{
  ClassBlock block = ClassBlock::build("Person").add_field("name", "string").add_field("age", "int");
  ostringstream oss;
  oss << block;

#if 0
  auto builder = CodeBuilder{"Person"}
    .add_field("name", "string")
    .add_field("age", "int");

  ostringstream oss;
  oss << builder;
#endif

  return 0;
}
