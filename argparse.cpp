
#include "test.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <tuple>

using namespace std;

ArgParser::ArgParser(char **begin, char **end, string configfile) {

  this->configfile = configfile;
  this->begin = begin;
  this->end = end;
}

bool ArgParser::exists(const string &option) {
  return find(this->begin, this->end, option) != this->end;
}

bool ArgParser::fexists(const string &option) {

  ifstream fs(configfile);
  string line;
  string key;
  string value;
  string delim = "=";

  while (getline(fs, line)) {
    int pos = line.find(delim);
    key = line.substr(0, pos);

    if (key == option)
      return true;

    value = line.substr(pos + 1);
  }

  return false;
};

string ArgParser::fget(const string &option) {

  ifstream fs(configfile);
  string line;
  string key;
  string value;
  string delim = "=";

  while (getline(fs, line)) {
    int pos = line.find(delim);
    key = line.substr(0, pos);
    if (key == option)
      value = line.substr(pos + 1);
    if (key == option) {
      return value;
    }
  }

  return "";
}

void ArgParser::arg(string longn, string shortn, string defaultv) {

  tuple<string, string, string> arg;

  get<0>(arg) = longn;
  get<1>(arg) = shortn;
  get<2>(arg) = defaultv;

  args.push_front(arg);
}

string ArgParser::aget(const string &option) {

  if (filesystem::exists(configfile)) {
    if (fexists(option.substr(2))) {
      return fget(option.substr(2));
    }
  }

  char **itr;
  string out;
  bool used_short = false;
  bool used_long = false;
  string find_str = "";

  for (tuple<string, string, string> &arg : args) {

    if (get<0>(arg) == option) {

      used_long = exists(get<0>(arg));
      used_short = exists(get<1>(arg));

      if (used_long)
        find_str = get<0>(arg);
      else if (used_short)
        find_str = get<1>(arg);

      if (!find_str.empty()) {
        // If user provided long or short name

        itr = find(this->begin, this->end, find_str);
        if (itr != end && ++itr != end)
          out = string(*itr);

      } else {
        // Else use default value
        out = get<2>(arg);
      }
    }
    find_str = "";
  }

  return out;
}

void ArgParser::show_help() {

  fexists(configfile);
  cout << "Usage:" << endl;

  cout << "live [OPTION]... -i [FILE]" << endl;
  cout << "live [OPTION]... -t [TYPESTRING] -p [PRINTSTRING]" << endl;
  cout << endl;

  for (tuple<string, string, string> arg : args) {
    cout << get<1>(arg) << ", " << get<0>(arg);

    if (!get<2>(arg).empty()) {
      cout << "\t\t\tDefault: " << get<2>(arg);
    }

    cout << endl;
  }

  cout << "#TYPE\n"
       << ".....\n"
       << ".....\n"
       << "###\n"
       << "#PRINT\n"
       << ".....\n"
       << ".....\n";
}

ConfigParser::ConfigParser(string filepath) {

  ifstream fs(filepath);
  string line;
  string key;
  string value;
  string delim = "=";

  while (getline(fs, line)) {

    int pos = line.find(delim);
    key = line.substr(0, pos);
    value = line.substr(pos);

    cout << key << endl;
    cout << value << endl;
  }
}

string ConfigParser::get(const string &option) { return "asdf"; }
