#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>

#include "MaeBlock.hpp"
#include "MaeConstants.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

using namespace schrodinger::mae;
using std::shared_ptr;

// We should make sure these do not exist before the tests starts,
// this will prevent any chances of finding the results of an older test.
const std::vector<std::string> generated_files = {"test_write.mae",
                                                  "test_write.maegz"};

class WriterGlobalFixture
{
  public:
    WriterGlobalFixture()
    {
        for (auto& file : generated_files) {
            boost::filesystem::path fpath(file);
            if (boost::filesystem::exists(fpath)) {
                boost::filesystem::remove(fpath);
            }
        }
    }
};

BOOST_GLOBAL_FIXTURE(WriterGlobalFixture);

BOOST_AUTO_TEST_SUITE(WriterSuite)

BOOST_AUTO_TEST_CASE(Writer0)
{
    Reader r("samples/test.mae");
    auto w = std::make_shared<Writer>("test_write.mae");
    std::vector<std::shared_ptr<Block>> input;

    std::shared_ptr<Block> b;
    while ((b = r.next(CT_BLOCK)) != nullptr) {
        input.push_back(b);
        w->write(b);
    }
    w.reset(); // Explicitly reset to flush file IO in writer

    Reader output_r("test_write.mae");
    int input_num = 0;
    while ((b = output_r.next(CT_BLOCK)) != nullptr) {
        BOOST_CHECK(*b == *(input[input_num++]));
    }
}

BOOST_AUTO_TEST_CASE(Writer1)
{
    Reader r("samples/test.mae");
    auto w = std::make_shared<Writer>("test_write.maegz");
    std::vector<std::shared_ptr<Block>> input;

    std::shared_ptr<Block> b;
    while ((b = r.next(CT_BLOCK)) != nullptr) {
        input.push_back(b);
        w->write(b);
    }
    w.reset(); // Explicitly reset to flush file IO in writer

    Reader output_r("test_write.maegz");
    int input_num = 0;
    while ((b = output_r.next(CT_BLOCK)) != nullptr) {
        BOOST_CHECK(*b == *(input[input_num++]));
    }
}

/*
// UNCOMMENT BLOCK TO TEST PERFORMANCE OF LIGAND WRITING
BOOST_AUTO_TEST_CASE(PerfTest)
{
    Reader r("samples/test.mae");
    auto w = std::make_shared<Writer>("test_write.maegz");
    std::vector<std::shared_ptr<Block> > input;

    std::shared_ptr<Block> b;
    while ((b = r.next(CT_BLOCK)) != nullptr) {
        input.push_back(b);
    }

    int total_write = 0;
    auto start = std::clock();
    for(int i=0; i<10000; i++) {
        for(const auto& b : input) {
            w->write(b);
            total_write++;
        }
    }
    auto duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"Runtime: "<< duration <<'\n' << " Structures: " << total_write;
    std::cout<<"Speed: "<< total_write/duration <<'\n';

}
*/

BOOST_AUTO_TEST_SUITE_END()
