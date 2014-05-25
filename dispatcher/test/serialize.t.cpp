#include "boost/test/unit_test.hpp"

#include "id_to_type.hpp"
#include "messages.hpp"

template <typename T>
void deserialize(const std::string&, T* msg)
{
  *msg = T();
}

auto deserializer = [] (TypeId id, const std::string& buffer)
{
  return Serializer<MsgHolder>::deserialize_tmp(static_cast<int>(id), buffer);
};

BOOST_AUTO_TEST_CASE(test_deserializer)
{
  auto msg_a = deserializer(TypeId::kMsgA, "");
  BOOST_REQUIRE(msg_a.which() == 0);
  auto msg_b = deserializer(TypeId::kMsgB, "");
  BOOST_REQUIRE(msg_b.which() == 1);
  auto msg_c = deserializer(TypeId::kMsgC, "");
  BOOST_REQUIRE(msg_c.which() == 2);
}
