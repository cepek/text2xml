#include "text2xml_record.h"

#include <algorithm>
#include <sstream>
#include <iostream>

namespace GNU_gama {
  namespace local {

    Text2xmlRecord::Text2xmlRecord(std::string line)
    {
      // remove everything starting from hash (input data comment)
      auto hash = line.find('#');
      if (hash != std::string::npos) line.erase(hash);

      // remove everything starting from apostrophe (inline record description)
      auto apostrophe = line.find('\'');
      if (apostrophe != std::string::npos) {
          note_ = line.substr(apostrophe+1);
          line.erase(apostrophe);
        }

      // check if some non-whitespace characters remained
      auto wsiter = std::find_if(line.begin(), line.end(),
                                 [](char c){return !std::iswspace(c);});
      if (wsiter == line.end()) return;

      std::istringstream istr(line);
      std::string word;
      istr >> word;
      if (word.empty()) return;   // this should never happen ...
      for (auto& c : word) c = std::toupper(c);
      code_ = word;

      // Separate characters '!', '*' and ';' by spaces where necessary
      std::string temp;
      std::getline(istr, temp);
      char previous = ' ';
      for (char c : temp) {
          if (!std::isspace(previous) && (c=='!' || c=='*' || c==';')) {
              data_.push_back(' ');
            }
          data_.push_back(c);
          previous = c;
        }

      // trim leading and trailing spaces
      const std::string WHITESPACE = " \n\r\t\f\v";
      auto trim = [&WHITESPACE](std::string& s)
      {
        auto ind = s.find_first_not_of(WHITESPACE);
        if (ind != std::string::npos) s = s.substr(ind);
        ind = s.find_last_not_of(WHITESPACE);
        if (ind != std::string::npos) s = s.substr(0,ind+1);
      };
      trim(data_);
      trim(note_);

      // tokens ...
      std::istringstream istrtok(data_);
      while (istrtok >> temp) {
          tokens_.push_back(temp);
        }
    }

    std::string Text2xmlRecord::code() const
    {
      return code_;
    }

    std::string Text2xmlRecord::note() const
    {
      return note_;
    }

    bool Text2xmlRecord::empty() const
    {
      return code_.empty();
    }

    std::string::size_type Text2xmlRecord::size() const
    {
      return tokens_.size();
    }

    const std::vector<std::string>& Text2xmlRecord::tokens() const
    {
      return tokens_;
    }

  } // namespace local
} // namespace GNU_gama
