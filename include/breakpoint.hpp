#pragma once

#include <linux/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <cstdint>

namespace minidbg {

class breakpoint {
public:
    breakpoint() = default;
    breakpoint(pid_t pid, std::intptr_t addr)
      : m_pid{pid}, m_addr{addr}, m_enabled{false}, m_saved_data{}
    {}

    void enable()
    {
        auto data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr, nullptr);
        m_saved_data = static_cast<uint8_t>(data & 0xFF); // save bottom byte
        uint64_t int3 = 0xCC;
        uint64_t data_with_int3 = ((data & ~0xFF) | int3); // set bottom byte to 0xCC
        ptrace(PTRACE_POKEDATA, m_pid, m_addr, data_with_int3);

        m_enabled = true;
    }
    void disable()
    {
        auto data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr, nullptr);
        auto restored_data = ((data & ~0xFF) | 0xFF);
        ptrace(PTRACE_POKEDATA, m_pid, m_addr, restored_data);

        m_enabled = false;
    }

    auto is_enabled() const -> bool { return m_enabled; }
    auto get_address() const -> std::intptr_t { return m_addr; }

private:
    pid_t m_pid;
    std::intptr_t m_addr;
    bool m_enabled;
    uint8_t m_saved_data; // data which used to be at the breakpoint address
};

} // minidbg
