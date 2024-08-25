#include <boost/ut.hpp>

#include <gnuradio-4.0/oot/oot_blocks.hpp>

const boost::ut::suite KnownBlockTests = [] {
    using namespace boost::ut;
    using namespace std::string_literals;

    "Registered"_test = [] {
        expect(true);
    };
};

int
main() {
    return boost::ut::cfg<boost::ut::override>.run();
}
