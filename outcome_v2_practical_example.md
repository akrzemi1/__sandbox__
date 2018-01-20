```c++
#include "outcome.hpp"
#include <boost/tokenizer.hpp>
#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

namespace out = OUTCOME_V2_NAMESPACE;
#define TRY(ex) OUTCOME_TRYX(ex)  // non portable feature of Boost.Outcome

// "1-10|11-20|21-99;1-10|10-50|51-99"
const boost::char_separator<char> sep_1 {";", "", boost::keep_empty_tokens};
const boost::char_separator<char> sep_2 {"|", "", boost::keep_empty_tokens};
const boost::char_separator<char> sep_3 {"-", "", boost::keep_empty_tokens};

typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

struct Range { int lo, hi; };
struct Distrib { Range a, b, c; };

struct BadInput
{
  enum {
    no_distrib = 1,
    range_count_not_3,
    distrib_starts_with_no_1,
    distrib_ends_with_no_99,
    gap_between_ranges,
    overlap_between_ranges,
    inverted_range,
    no_2_numbers_in_range,
    not_a_number,
  }   type;
  int val1;
  int val2;
};

std::string message(const BadInput& e)
{
  std::ostringstream ostr;
  ostr << "E" << e.type << "(" << e.val1 << ", " << e.val2 << ")";
  return ostr.str();
}

struct boost::cnv::by_default : boost::cnv::lexical_cast {};

out::result<int, BadInput> parse_int (const std::string& input)
{
  if (boost::optional<int> i = boost::convert<int>(input))
    return *i;
  else
    return out::failure(BadInput{BadInput::not_a_number});
}

out::result<Range, BadInput> parse_range (const std::string& input)
{
  tokenizer numbers_s {input, sep_3};
  auto it = numbers_s.begin(), itEnd = numbers_s.end();
  int count = std::distance(it, itEnd);
 
  if (count != 2)
    return out::failure(BadInput{BadInput::no_2_numbers_in_range, count});
   
  Range r {TRY(parse_int(*it++)), TRY(parse_int(*it++))};
 
  if (r.lo > r.hi)
    return out::failure(BadInput{BadInput::inverted_range, r.lo, r.hi});
   
  return r;
}

out::result<Distrib, BadInput> parse_distrib (const std::string& input)
{
  tokenizer ranges_s {input, sep_2};
  auto it = ranges_s.begin(), itEnd = ranges_s.end();
  int count = std::distance(it, itEnd);
 
  if (count != 3)
    return out::failure(BadInput{BadInput::range_count_not_3, count});
 
  Distrib d {TRY(parse_range(*it++)), TRY(parse_range(*it++)), TRY(parse_range(*it++))};
 
  if (d.a.lo != 1)
    return out::failure(BadInput{BadInput::distrib_starts_with_no_1, d.a.lo});
   
  if (d.a.hi < d.b.lo - 1)
    return out::failure(BadInput{BadInput::gap_between_ranges, d.a.hi, d.b.lo});
   
  if (d.a.hi > d.b.lo - 1)
    return out::failure(BadInput{BadInput::overlap_between_ranges, d.a.hi, d.b.lo});
   
  if (d.b.hi < d.c.lo - 1)
    return out::failure(BadInput{BadInput::gap_between_ranges, d.b.hi, d.c.lo});
   
  if (d.b.hi > d.c.lo - 1)
    return out::failure(BadInput{BadInput::overlap_between_ranges, d.b.hi, d.c.lo});
   
   if (d.c.hi != 99)
    return out::failure(BadInput{BadInput::distrib_ends_with_no_99, d.c.hi});
   
  return std::move(d);
}

out::result<std::vector<Distrib>, BadInput> parse (const std::string& input)
{
  out::result<std::vector<Distrib>, BadInput> ans {out::in_place_type<std::vector<Distrib>>};
  std::vector<Distrib> & out_ranges = ans.value(); // will internal if be ellided?
 
  tokenizer distribs_s {input, sep_1};
  for (const std::string& distrib_s : distribs_s)
    out_ranges.push_back(TRY(parse_distrib(distrib_s)));
     
  if (out_ranges.empty())
    return out::failure(BadInput{BadInput::no_distrib});
   
  return ans;
}

void process (const std::string& input)
{
  out::result<std::vector<Distrib>, BadInput> data = parse(input);
  if (data)
    std::cout << "ok, distributions: " << data.value().size() << std::endl;
  else
    std::cout << message(data.error()) << std::endl;
}

int main ()
{
  process("1-10|11-20|21-99;1-10|11-50|51-99");
  process("1-10|11-20|21-99");
  process("1-10|11-20|21-99;");
  process("1-10|11-20|91-99;1-10|11-50|51-99");
  process("1-10|10-20|21-99;1-10|11-50|51-99");
}
```
