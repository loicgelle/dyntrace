/**
 * Global tracepoint data for x86_64.
 */
#ifndef DYNTRACE_FASTTP_ARCH_X86_64_CONTEXT_HPP_
#define DYNTRACE_FASTTP_ARCH_X86_64_CONTEXT_HPP_

#include <memory>
#include <unordered_set>
#include <vector>

#include <arch/arch.hpp>
#include <fasttp/code_ptr.hpp>
#include <fasttp/common.hpp>
#include <process/process.hpp>
#include <util/integer_range.hpp>
#include <util/locked.hpp>

#include "code_allocator.hpp"

namespace dyntrace::fasttp
{
    class context;
    class arch_context;
    class arch_tracepoint;

    /**
     * Handle that represents a trap redirection.
     * When this class is destroyed, the redirection is removed from the active list.
     * When a trap is hit, it checks for the active redirections.
     * If found, it calls a handler and then sets rip to the redirection. Else it calls the old handler.
     * **DOES NOT WORK WITH GDB**. GDB ignores the fact that a trap handler could be set by the debugged program.
     */
    class redirect_handle
    {
    public:
        redirect_handle(const redirect_handle&) = delete;
        redirect_handle& operator=(const redirect_handle&) = delete;

        redirect_handle(code_ptr at) noexcept
            : _at{at} {}

        redirect_handle(redirect_handle&& h) noexcept
            : _at{h._at}
        {
            h._at = {};
        }

        redirect_handle& operator=(redirect_handle&& h) noexcept
        {
            remove();
            _at = h._at;
            h._at = {};
            return *this;
        }

        ~redirect_handle()
        {
            remove();
        }

        void remove();

    private:
        code_ptr _at;
    };

    /**
     * Global data for tracepoints.
     */
    class arch_context
    {
        friend class redirect_handle;

        using allocator_type = dyntrace::locked<code_allocator>;
    public:
        explicit arch_context(context* ctx);
        ~arch_context();

        redirect_handle add_redirect(arch_tracepoint* tp, code_ptr at, code_ptr redirect);

        allocator_type::proxy_type allocator() noexcept
        {
            return _allocator.lock();
        }

    private:

        void remove_redirect(code_ptr at);

        std::unordered_set<code_ptr, code_ptr::hash> _redirects;
        allocator_type _allocator;
    };
}

extern "C" void __tracepoint_handler() noexcept;
extern const uintptr_t __tracepoint_handler_size;

#endif