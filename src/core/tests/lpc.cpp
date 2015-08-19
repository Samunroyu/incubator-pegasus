/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Microsoft Corporation
 * 
 * -=- Robust Distributed System Nucleus (rDSN) -=- 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

# include <dsn/internal/aio_provider.h>
# include <gtest/gtest.h>
# include <dsn/service_api_cpp.h>
# include "test_utils.h"

DEFINE_TASK_CODE(LPC_TEST_HASH, TASK_PRIORITY_COMMON, ::dsn::THREAD_POOL_DEFAULT)

void on_lpc_test_hash(void* p)
{
    std::string& result = *(std::string*)p;
    result = ::dsn::task::get_current_worker()->name();
}

void on_lpc_test_hash2(void* p)
{

}

TEST(core, lpc)
{
    std::string result;
    auto t = dsn_task_create(LPC_TEST_HASH, on_lpc_test_hash, (void*)&result, 1);
    dsn_task_add_ref(t);
    dsn_task_call(t, nullptr, 0);
    bool r = dsn_task_wait(t);
    dsn_task_release_ref(t);

    EXPECT_TRUE(r);
    EXPECT_TRUE(result.substr(result.length() - 9) == "default.1");
    
    t = dsn_task_create(LPC_TEST_HASH, on_lpc_test_hash2, nullptr, ::dsn::task::get_current_worker_index());
    dsn_task_add_ref(t);
    dsn_task_call(t, nullptr, 0);
    r = dsn_task_wait_timeout(t, 1000);
    dsn_task_release_ref(t);
    EXPECT_TRUE(!r);
}
