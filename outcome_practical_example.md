# A practical example of using Boost.Outcome for non-performance critical things.

## The task to be implemented

Function `parse` validates the input text if it matches the required form, and builds a corresponding data structure. One example of a correct input:

```
"1-10|11-20|21-99;1-10|10-50|51-99"
```

A `;` separates 'distributions'. Each distribution 'covers' numbers from 1 to 99 using 'ranges'.
Ranges are separated with a `|`. There are exactly 3 rages in every distribution. Each 'range' consists of two integers: denoting minimum and maximum value (inclusive).
Ranges cannot overlap, and no holes between ranges are allowed.
First range in any distribution always starts at 1, last ends at 99.

The allowed syntax in EBNF notation:

```
valid_input  ::= distribution (";" distribution)*
distribution ::= range "|" range "|" range
range        ::= integer "-" integer
```

The goal of the validation is not only to report a binary success or failure,
but to give the user a good feedback on what in paricular is considered wrong with her input.

### Constraints

Note that I am creating a lot of stringc underway. They may throw `std::bad_alloc`. I am ok with that. I do not want to avoid any exceptions. I want to report validation failures in `expected` and other run-time situations like resurce shortage are dealt through stack unwinding.

This is validation. It is not a performance-critical component.

## The source code
```c++
#include <boost/outcome.hpp>
#include <boost/tokenizer.hpp>
#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

namespace out = BOOST_OUTCOME_V1_NAMESPACE;
#define TRY(ex) BOOST_OUTCOME_TRYX(ex)  // non portable feature of Boost.Outcome

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

out::expected<int, BadInput> parse_int (const std::string& input)
{
  if (boost::optional<int> i = boost::convert<int>(input))
    return *i;
  else
    return out::make_unexpected(BadInput{BadInput::not_a_number});
}

out::expected<Range, BadInput> parse_range (const std::string& input)
{
  tokenizer numbers_s {input, sep_3};
  auto it = numbers_s.begin(), itEnd = numbers_s.end();
  int count = std::distance(it, itEnd);
 
  if (count != 2)
    return out::make_unexpected(BadInput{BadInput::no_2_numbers_in_range, count});
   
  Range r {TRY(parse_int(*it++)), TRY(parse_int(*it++))};
 
  if (r.lo > r.hi)
    return out::make_unexpected(BadInput{BadInput::inverted_range, r.lo, r.hi});
   
  return r;
}

out::expected<Distrib, BadInput> parse_distrib (const std::string& input)
{
  tokenizer ranges_s {input, sep_2};
  auto it = ranges_s.begin(), itEnd = ranges_s.end();
  int count = std::distance(it, itEnd);
 
  if (count != 3)
    return out::make_unexpected(BadInput{BadInput::range_count_not_3, count});
 
  Distrib d {TRY(parse_range(*it++)), TRY(parse_range(*it++)), TRY(parse_range(*it++))};
 
  if (d.a.lo != 1)
    return out::make_unexpected(BadInput{BadInput::distrib_starts_with_no_1, d.a.lo});
   
  if (d.a.hi < d.b.lo - 1)
    return out::make_unexpected(BadInput{BadInput::gap_between_ranges, d.a.hi, d.b.lo});
   
  if (d.a.hi > d.b.lo - 1)
    return out::make_unexpected(BadInput{BadInput::overlap_between_ranges, d.a.hi, d.b.lo});
   
  if (d.b.hi < d.c.lo - 1)
    return out::make_unexpected(BadInput{BadInput::gap_between_ranges, d.b.hi, d.c.lo});
   
  if (d.b.hi > d.c.lo - 1)
    return out::make_unexpected(BadInput{BadInput::overlap_between_ranges, d.b.hi, d.c.lo});
   
   if (d.c.hi != 99)
    return out::make_unexpected(BadInput{BadInput::distrib_ends_with_no_99, d.c.hi});
   
  return std::move(d);
}

out::expected<std::vector<Distrib>, BadInput> parse (const std::string& input)
{
  out::expected<std::vector<Distrib>, BadInput> ans {out::value};
  std::vector<Distrib> & out_ranges = ans.value(); // will internal if be ellided?
 
  tokenizer distribs_s {input, sep_1};
  for (const std::string& distrib_s : distribs_s)
    out_ranges.push_back(TRY(parse_distrib(distrib_s)));
     
  if (out_ranges.empty())
    return out::make_unexpected(BadInput{BadInput::no_distrib});
   
  return ans;
}

void process (const std::string& input)
{
  out::expected<std::vector<Distrib>, BadInput> data = parse(input);
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
