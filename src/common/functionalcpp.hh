#ifndef FUNCTIONALCPP_HH

#include <vector>

// copied from https://gist.github.com/phatak-dev/766eccf8c72484ad623b

namespace dqmcpp {
namespace common {

template <typename Collection, typename unop>
void for_each(Collection col, unop op) {
  std::for_each(col.begin(), col.end(), op);
}

template <typename T, typename K, typename unop>
std::vector<K> map(std::vector<T>& col, unop op) {
  std::vector<K> _tmp;
  _tmp.reserve(col.size());
  for (auto& e : col)
    _tmp.push_back(op(e));
  return _tmp;
}

template <typename Collection, typename binop>
Collection zip(Collection fc, Collection sc, binop op) {
  std::transform(fc.begin(), fc.end(), sc.begin(), fc.begin(), op);
  return fc;
}

template <typename Collection, typename Condition>
bool exists(Collection col, Condition con) {
  auto exist = std::find_if(col.begin(), col.end(), con);
  return exist != col.end();
}

template <typename Collection, typename Predicate>
Collection filterNot(Collection col, Predicate predicate) {
  auto returnIterator = std::remove_if(col.begin(), col.end(), predicate);
  col.erase(returnIterator, std::end(col));
  return col;
}

template <typename Collection, typename Predicate>
Collection filter(Collection col, Predicate predicate) {
  auto fnCol = filterNot(col, [predicate](typename Collection::value_type i) {
    return !predicate(i);
  });
  return fnCol;
}

}  // namespace common
}  // namespace dqmcpp

#define FUNCTIONALCPP_HH
#endif