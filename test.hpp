#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

class ConfigParser {

private:
  string filepath;
  list<tuple<string, string>> args;

public:
  ConfigParser(string filepath);
  string get(const string &option);
};

class ArgParser {

private:
  char **begin;
  char **end;
  list<tuple<string, string, string>> args;
  string configfile;

public:
  ArgParser(char **begin, char **end, string configfile);
  bool exists(const string &option);
  bool fexists(const string &option);
  string aget(const string &option);
  string fget(const string &option);
  void show_help();
  void arg(string longn, string shortn, string defaultv);
};

class Leaf {

public:
  string type;
  string attributes;
  Leaf();
  Leaf(string type, string attributes);
  Leaf *attr(string key, string value);
  void dump();
};

class Stem {

public:
  string type;
  string attributes;
  string contents;
  vector<Stem> nodes;
  vector<Leaf> snodes;
  Stem();
  Stem(string type, string attributes, string contents);
  vector<Stem, allocator<Stem>>::iterator stem(Stem n);
  Stem *attr(string key, string value);
  vector<Stem, allocator<Stem>>::iterator stem(string type, string attributes,
                                               string contents);
  vector<Leaf, allocator<Leaf>>::iterator leaf(Leaf n);
  vector<Leaf, allocator<Leaf>>::iterator leaf(string type, string attributes);
  void dump();
};

class KeyFrames {

public:
  stringstream content;
  int iter;
  KeyFrames();
  void add_pct(double pct);
  Stem to_stem(string bghexc, string fghexc);
};

list<tuple<string, double>> fileio(string filename, double speedm);

list<tuple<string, double>> inputio(string type, string print, double speedm);
