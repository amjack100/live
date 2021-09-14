
#include "test.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <tuple>

using namespace std;

list<tuple<string, double>> fileio(string filename, double speedm) {

  const string typeit = "#TYPE";
  const string printit = "#PRINT";
  const string stopit = "###";
  list<tuple<string, double>> str_times;

  auto sa = 0.05;
  auto sb = 0.1;
  auto sc = 0.3;
  auto sd = 0.5;
  auto se = 0.7;

  double accum = 0.0;

  // Fastest
  uniform_real_distribution<double> unif(sa * speedm, sb * speedm);
  uniform_real_distribution<double> unif2(sb * speedm, sc * speedm);
  uniform_real_distribution<double> unif3(sc * speedm, sd * speedm);
  uniform_real_distribution<double> unif4(sd * speedm, se * speedm);
  // Slowest

  default_random_engine re;

  ifstream infile(filename);

  string s;
  bool flag_type = false;
  bool flag_print = false;
  bool flag_printstart = false;
  string line;

  while (getline(infile, line)) {
    tuple<string, double> str_time;

    if (line == typeit) {
      flag_type = true;
      continue;
    }

    if (line == printit) {
      flag_print = true;
      flag_printstart = true;
      continue;
    }

    if (line == stopit) {
      flag_type = false;
      flag_print = false;
      continue;
    }

    if (flag_type) {
      for (char c : line) {
        string str;
        str.push_back(c);

        if (str == ";")
          accum += unif3(re);
        else if (str == "$")
          accum += 0;
        else if (str == " ")
          accum += unif2(re);
        else
          accum += unif(re);

        str_times.push_back(make_tuple(str, accum));
      }
    } else if (flag_print) {

      if (flag_printstart) {
        // First encounter with #PRINT triggers slight delay,
        // others do not - makes multiple lines dump at once
        accum += unif4(re);
        flag_printstart = false;
      }

      str_times.push_back(make_tuple("\n" + line, accum));
    }
  }

  // Add a dummy " " with additional time at end so animation doesnt
  // restart instantly

  accum += 20;
  str_times.push_back(make_tuple(" ", accum));
  return str_times;
};

list<tuple<string, double>> inputio(string type, string print, double speedm) {

  list<tuple<string, double>> str_times;

  auto sa = 0.05;
  auto sb = 0.1;
  auto sc = 0.3;
  auto sd = 0.5;
  auto se = 0.7;

  double accum = 0.0;

  // Fastest
  uniform_real_distribution<double> unif(sa * speedm, sb * speedm);
  uniform_real_distribution<double> unif2(sb * speedm, sc * speedm);
  uniform_real_distribution<double> unif3(sc * speedm, sd * speedm);
  uniform_real_distribution<double> unif4(sd * speedm, se * speedm);
  // Slowest

  default_random_engine re;

  for (char c : type) {
    string str;
    str.push_back(c);

    if (str == ";")
      accum += unif3(re);
    else if (str == "$")
      accum += 0;
    else if (str == " ")
      accum += unif2(re);
    else
      accum += unif(re);
    str_times.push_back(make_tuple(str, accum));
  }

  accum += unif4(re);
  str_times.push_back(make_tuple("" + print, accum));

  accum += 20;
  str_times.push_back(make_tuple(" ", accum));
  return str_times;
};
