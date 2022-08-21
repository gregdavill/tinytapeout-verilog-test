#include "harness.h"

harness::harness(void)
{
    top.step();

    top.p_i__rst.set<bool>(true);
    top.step();
    top.step();
    top.p_i__rst.set<bool>(false);

    th = new std::thread(&harness::thread_func, this);
}

void harness::step(void)
{
    m.lock();

    cycles++;

    top.p_i__clk.set(false);
    top.step();
    top.p_i__clk.set(true);
    top.step();

    sr_logic();

    m.unlock();
}

void harness::i_set_m(bool v)
{
    m.lock();
    top.p_i__set__m.set<bool>(v);
    m.unlock();
}
void harness::i_set_h(bool v)
{
    m.lock();
    top.p_i__set__h.set<bool>(v);
    m.unlock();
}

int harness::get_cycles()
{
    int _cycles;
    m.lock();
    _cycles = cycles;
    m.unlock();
    return _cycles;
}

void harness::sr_logic()
{
    static bool clk_r = false;
    static bool latch_r = false;

    bool clk = top.p_o__clk.get<bool>();
    bool latch = top.p_o__latch.get<bool>();
    bool bit = top.p_o__bit.get<bool>();

    /* Rising latch */
    if (latch & !latch_r)
    {
        for (int i = 0; i < 6; i++)
        {
            seg[i] = sr_unlatch[i];
        }
    }

    /* Rising clock */
    if (clk & !clk_r)
    {
        bool carry = bit;
        for (int i = 0; i < (6); i++)
        {
            bool new_carry = sr_unlatch[i] & 0x80 ? 1 : 0;
            sr_unlatch[i] = (sr_unlatch[i] << 1) | carry;

            carry = new_carry;
        }
    }

    /* Used for edge detection */
    clk_r = clk;
    latch_r = latch;
}

void harness::thread_func()
{
    while (true)
    {
        step();
        std::this_thread::sleep_for(std::chrono::milliseconds(4)); /* ~256Hz */
    }
}