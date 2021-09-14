#include "test.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

Leaf::Leaf(){};
Leaf::Leaf(std::string type, std::string attributes) {
  this->type = type;
  this->attributes = attributes;
}

void Leaf::dump() {

  cout << "<" << this->type << " ";
  cout << this->attributes;
  cout << "/>";
}

Stem::Stem(string type, string attributes, string contents) {
  // Optional <svg> -> svg
  if (type.substr(0, 1) == "<" &&
      type.substr(type.length() - 1, type.length()) == ">") {

    type = type.substr(1, type.length() - 2);
  }
  // type = type.substr(1, type.length() - 1);
  this->type = type;
  this->attributes = attributes;
  this->contents = contents;
}

Stem::Stem(){};
std::vector<Stem, std::allocator<Stem>>::iterator Stem::stem(Stem n) {
  return nodes.insert(nodes.begin(), n);
}

Stem *Stem::attr(string key, string value) {
  attributes += (" " + key + "=" + "\"" + value + "\"");
  return this;
}

Leaf *Leaf::attr(string key, string value) {
  attributes += (" " + key + "=" + "\"" + value + "\"");
  return this;
}

std::vector<Stem, std::allocator<Stem>>::iterator
Stem::stem(string type, string attributes, string contents) {
  Stem s = Stem(type, attributes, contents);
  return this->stem(s);
}

std::vector<Leaf, std::allocator<Leaf>>::iterator Stem::leaf(Leaf n) {
  return snodes.insert(snodes.begin(), n);
}

std::vector<Leaf, std::allocator<Leaf>>::iterator
Stem::leaf(string type, string attributes) {
  return this->leaf(Leaf(type, attributes));
}

void Stem::dump() {
  cout << "<" << type << attributes << ">";
  cout << this->contents;
  for (Leaf o : snodes)
    o.dump();

  for (Stem c : nodes)
    c.dump();
  cout << "</" << this->type << ">";
}

KeyFrames::KeyFrames() {

  this->content << "@keyframes e{";
  this->content << "0%{transform:translateX(0)}";
  this->iter = 0.0;
};

void KeyFrames::add_pct(double pct) {

  this->iter++;
  this->content << pct << "%";
  this->content << "{transform:translateX(" << this->iter * -136 << "px)}";
}

Stem KeyFrames::to_stem(string bghexc, string fghexc) {

  //#282d35
  //#b9c0cb
  this->content << "to{transform:translateX(" << this->iter * -136 << ")}";
  this->content << "}.a{fill:" << bghexc << "}.g{fill:" << fghexc
                << ";white-space:pre}";
  return Stem("style", "", this->content.str());
};
