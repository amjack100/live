
#include "test.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <tuple>

using namespace std;

Stem construct__g(list<string> s, double totalsec, double itercnt, int padx,
                  int pady) {

  Stem _g;
  stringstream gfmt;

  // The first pass is unique for the cursor block and contains
  // no text

  auto s_txt = Stem("svg", "", "");
  s_txt.leaf("use", "")
      ->attr("xlink:href", "#b")
      ->attr("x", to_string(0.004 + padx));
  s_txt.leaf("use", "")->attr("xlink:href", "#a");

  auto s_gs = Stem("svg", "", "");
  s_gs.attr("width", to_string(s.size() * 136));
  s_gs.stem(s_txt);

  // Subsequent passes contain text
  int i = 0;
  int cursorpos = 0;
  string text;
  stringstream ss;
  Stem s_innertxt;
  for (string const &c : s) {

    s_txt = Stem("svg", "", "");

    s_txt.attr("x", to_string((i + 1) * 136));

    s_innertxt = Stem("text", "", "");
    s_innertxt.attr("y", to_string(1.67 + pady))
        ->attr("class", "g")
        ->attr("x", to_string(padx));

    if (c.length() == 1) {
      cursorpos++;
      ss << c;
      text = ss.str() + "&#9608;";
    } else {
      ss << "&#9608;"; // Cursor esc character
      // ss << "&#10;";   // Newline esc character
      // ss << c;
      // text = ss.str();
    }
    if (c.length() > 1) {
      s_innertxt.stem("tspan", "", c)
          ->attr("x", to_string(padx))
          ->attr("dy", "1.2em");
    }

    s_innertxt.stem("tspan", "", text)->attr("x", to_string(padx));

    s_txt.stem(s_innertxt);

    s_txt.stem("text", "", "")
        ->attr("y", to_string(1.67 + pady))
        ->attr("class", "g")
        ->attr("x", to_string(padx));

    i++;
    s_gs.stem(s_txt);
  }

  gfmt << " style=\"animation-duration:" << totalsec
       << "s;animation-iteration-count:";

  if (itercnt < 0)
    gfmt << "infinite";
  else
    gfmt << itercnt;

  gfmt << ";animation-name:e;animation-timing-function:steps(1,end)\"";

  _g = Stem("g", gfmt.str(), "");
  _g.stem(s_gs);

  return _g;
}

Stem construct_g(list<string> s, double totalsec, double itercnt,
                 string fontfam) {
  Stem g = Stem("g", "", "");

  g.attr("font-family", fontfam)->attr("font-size", "0.1em");

  Stem gd = Stem("defs", "", "");
  Stem gds1 = *Stem("symbol", "", "").attr("id", "a");

  gds1.leaf("path", "")
      ->attr("fill", "transparent")
      ->attr("d", "M0 0h136v62H0z");
  Stem gds2 = *Stem("symbol", "", "").attr("id", "b");

  gds2.leaf("path", "")
      ->attr("fill", "#6f7683")
      ->attr("d", "M0 0h1.102v2.171H0z");

  // Defines cursor color ^^

  gd.stem(gds1);
  gd.stem(gds2);
  g.leaf("path", "")->attr("class", "a")->attr("d", "M0 0h136v134.602H0z");
  g.stem(gd);

  return g;
};

int main(int argc, char *argv[]) {

  ArgParser opt(argv, argv + argc, string(getenv("HOME")) + "/.liveconfig");

  opt.arg("--width", "-w", "500");
  opt.arg("--height", "-h", "200");
  opt.arg("--input", "-i", "");
  opt.arg("--repeat", "-r", "-1");
  opt.arg("--fore", "-f", "#b9c0cb");
  opt.arg("--back", "-b", "#282d35");
  opt.arg("--speed", "-s", "1");
  opt.arg("--type", "-t", "");
  opt.arg("--print", "-p", "");
  opt.arg("--padx", "-x", "0");
  opt.arg("--pady", "-y", "0");
  opt.arg("--config", "-c", string(getenv("HOME")) + "/.liveconfig");
  opt.arg("--font", "-z",
          "Consolas, Monaco, 'Andale Mono', 'Ubuntu Mono', monospace");

  if (argc == 1) {
    opt.show_help();
    exit(0);
  }
  // if (filesystem::exists(opt.aget("--config"))) {
  //}

  auto height = stoi(opt.aget("--height"));
  auto width = stoi(opt.aget("--width"));
  auto swidth = to_string(width);
  auto sheight = to_string(height);
  auto padx = stoi(opt.aget("--padx"));
  auto pady = stoi(opt.aget("--pady"));
  auto repeat = stod(opt.aget("--repeat"));
  auto bg = opt.aget("--back");
  auto fg = opt.aget("--fore");
  auto inputf = opt.aget("--input");
  auto speedm = stod(opt.aget("--speed"));
  auto typetxt = opt.aget("--type");
  auto printtxt = opt.aget("--print");
  auto fontfam = opt.aget("--font");

  list<tuple<string, double>> lstr;

  if (!typetxt.empty() && !printtxt.empty()) {
    lstr = inputio(typetxt, printtxt, speedm);
  } else if (!inputf.empty()) {
    lstr = fileio(inputf, speedm);
  } else {
    opt.show_help();
    exit(0);
  }

  auto max = get<1>(lstr.back());
  KeyFrames kf = KeyFrames();
  list<string> strs;

  for (auto tup : lstr) {
    strs.push_back(get<0>(tup));
    double time = get<1>(tup);
    kf.add_pct(time / max * 100);
  }

  auto root = Stem("svg", "", "");
  root.attr("xmlns", "http://www.w3.org/2000/svg")
      ->attr("xmlns:xlink", "http://www.w3.org/1999/xlink")
      ->attr("height", sheight)
      ->attr("width", swidth);

  auto rect = Leaf("rect", "");
  rect.attr("width", swidth)
      ->attr("height", sheight)
      ->attr("rx", "0")
      ->attr("ry", "0")
      ->attr("class", "a");
  root.leaf(rect);

  auto inner = Stem("svg", "", "");
  inner.attr("xmlns", "http://www.w3.org/2000/svg")
      ->attr("xmlns:xlink", "http://www.w3.org/1999/xlink")
      ->attr("height", sheight)
      ->attr("width", swidth)
      ->attr("viewBox",
             "0 0 " + to_string(width / 10) + " " + to_string(height / 10));

  auto g = construct_g(strs, max, repeat, fontfam);
  g.stem(construct__g(strs, max, repeat, padx, pady));

  inner.stem(g)->stem(kf.to_stem(bg, fg));

  root.stem(inner);
  root.dump();
  // root.stem(inner)->dump();
}
